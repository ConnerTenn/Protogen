
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include <libv4l2.h>
#include <errno.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer 
{
	void   *start;
	size_t length;
};

static void xioctl(int fd, int request, void *arg)
{
	int r;

	do { r = v4l2_ioctl(fd, request, arg); } 
	while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	if (r == -1) { fprintf(stderr, "error %d, %s\n", errno, strerror(errno)); exit(EXIT_FAILURE); }
}

int main()
{
	struct v4l2_buffer buf;
	
	int fd = v4l2_open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (fd < 0) { perror("Cannot open device"); exit(EXIT_FAILURE); }

	struct v4l2_format fmt;
	CLEAR(fmt);
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	xioctl(fd, VIDIOC_S_FMT, &fmt);
	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24) { printf("Libv4l didn't accept RGB24 format. Can't proceed.\n"); exit(EXIT_FAILURE); }
	if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480)) { printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height); }

	struct v4l2_requestbuffers req;
	CLEAR(req);
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	xioctl(fd, VIDIOC_REQBUFS, &req);

	struct buffer *buffers = calloc(req.count, sizeof(*buffers));
	int numBuffers = req.count;
	for (int i=0; i<numBuffers; ++i) 
	{
		CLEAR(buf);

		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;

		xioctl(fd, VIDIOC_QUERYBUF, &buf);

		buffers[i].length = buf.length;
		buffers[i].start = v4l2_mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buf.m.offset);

		if (MAP_FAILED == buffers[i].start) { perror("mmap"); exit(EXIT_FAILURE); }

		printf("Buff %d\n", i);
	}

	for (int i = 0; i < numBuffers; ++i) 
	{
		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		xioctl(fd, VIDIOC_QBUF, &buf);
	}
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	xioctl(fd, VIDIOC_STREAMON, &type);
	for (int i = 0; i < 20; i++) 
	{
		struct timeval tv;
		int r;
		do 
		{
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(fd, &fds);

			/* Timeout. */
			tv.tv_sec = 2;
			tv.tv_usec = 0;

			r = select(fd + 1, &fds, NULL, NULL, &tv);
		} 
		while ((r == -1 && (errno = EINTR)));

		if (r == -1) { perror("select"); return errno; }

		CLEAR(buf);
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		xioctl(fd, VIDIOC_DQBUF, &buf);

		char out_name[256];
		sprintf(out_name, "out%03d.ppm", i);
		FILE *fout = fopen(out_name, "w");
		if (!fout) { perror("Cannot open image"); exit(EXIT_FAILURE); }
		fprintf(fout, "P6\n%d %d 255\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
		fwrite(buffers[buf.index].start, buf.bytesused, 1, fout);
		fclose(fout);

		xioctl(fd, VIDIOC_QBUF, &buf);
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	xioctl(fd, VIDIOC_STREAMOFF, &type);
	for (int i = 0; i < numBuffers; ++i)
	{
		v4l2_munmap(buffers[i].start, buffers[i].length);
	}
	v4l2_close(fd);

	return 0;
}
