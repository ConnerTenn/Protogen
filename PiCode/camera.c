

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include <libv4l2.h>
#include <errno.h>

#include "camera.h"

static int xioctl(int fd, int request, void *arg)
{
	int r;

	do { r = v4l2_ioctl(fd, request, arg); } 
	while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	return r;
}

void InitCamera(int *camfd, u_int8_t **buffer)
{
	*camfd = v4l2_open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (*camfd < 0) { perror("Cannot open device"); exit(EXIT_FAILURE); }

	struct v4l2_format fmt = {0};
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = CAMWIDTH;
	fmt.fmt.pix.height      = CAMHEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24; //V4L2_PIX_FMT_RGB24
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	if (xioctl(*camfd, VIDIOC_S_FMT, &fmt)==-1) { perror("Setting Pixel Format"); exit(-1); }
	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_BGR24) { printf("Libv4l didn't accept format. Can't proceed.\n"); exit(EXIT_FAILURE); }
	if ((fmt.fmt.pix.width != CAMWIDTH) || (fmt.fmt.pix.height != CAMHEIGHT)) { printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height); }


	struct v4l2_requestbuffers req ={0};
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (xioctl(*camfd, VIDIOC_REQBUFS, &req)==-1) { perror("Requesting Buffer"); exit(-1); }


	struct v4l2_buffer buf = {0};
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;

	xioctl(*camfd, VIDIOC_QUERYBUF, &buf);

	size_t bufferLen = buf.length;
	*buffer = v4l2_mmap(NULL, bufferLen, PROT_READ | PROT_WRITE, MAP_SHARED, *camfd, buf.m.offset);
	if (*buffer == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	if( xioctl(*camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
	
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl(*camfd, VIDIOC_STREAMON, &type)==-1) { perror("Start Capture"); exit(-1); }
}

void DestroyCamera(int camfd, u_int8_t **buffer)
{
	struct v4l2_buffer buf = {0};
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;

	xioctl(camfd, VIDIOC_QUERYBUF, &buf);
	size_t bufferLen = buf.length;

	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	if (xioctl(camfd, VIDIOC_STREAMOFF, &type)==-1) { perror("Stop Capture"); exit(-1); }
	v4l2_munmap(*buffer, bufferLen);
	v4l2_close(camfd);
}

void QueueBuffer(int camfd)
{
	struct v4l2_buffer buf = {0};

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;

	if( xioctl(camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
}

void DeQueueBuffer(int camfd)
{
	struct v4l2_buffer buf;
	
	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;

	xioctl(camfd, VIDIOC_DQBUF, &buf);
}