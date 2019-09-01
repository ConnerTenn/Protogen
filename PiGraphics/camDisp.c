
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <signal.h>
#include <sys/kd.h>

#include <linux/videodev2.h>
#include <libv4l2.h>
#include <errno.h>

#define FB_SIZE (480*800*4)


struct PixelData
{
	uint8_t B;
	uint8_t G;
	uint8_t R;
	uint8_t Empty;
};

union Ksequ
{
	uint8_t seq[8];
	uint64_t val;
};

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
	struct termios new_termios;

	// take two copies - one for now, one for later
	tcgetattr(0, &orig_termios);
	memcpy(&new_termios, &orig_termios, sizeof(new_termios));

	// register cleanup handler, and set the new terminal mode
	atexit(reset_terminal_mode);
	//cfmakeraw(&new_termios);
	new_termios.c_lflag &= ~(ECHO | ECHONL| ICANON | IEXTEN);
	
	//new_termios.c_oflag |= ONLCR;
	//new_termios.c_lflag |= ISIG;
	tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

uint8_t getch()
{
	int r;
	uint8_t c;
	if ((r = read(0, &c, sizeof(c))) < 0) { return (uint8_t)r; }
	else { return c; }
}

static int xioctl(int fd, int request, void *arg)
{
	int r;

	do { r = v4l2_ioctl(fd, request, arg); } 
	while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	return r;
}

uint8_t Run = 1;

void InteruptHandler(int arg) { reset_terminal_mode(); Run=0; }

int main()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	ioctl(0, KDSETMODE, KD_GRAPHICS);
	set_conio_terminal_mode();

	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	uint8_t *fb0 = 0;
	fb0 = mmap(0, FB_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED,dispfd,0);
	close(dispfd);


	
	int camfd = v4l2_open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
	if (camfd < 0) { perror("Cannot open device"); exit(EXIT_FAILURE); }

	struct v4l2_format fmt = {0};
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width       = 640;
	fmt.fmt.pix.height      = 480;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24; //V4L2_PIX_FMT_RGB24
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	if (xioctl(camfd, VIDIOC_S_FMT, &fmt)==-1) { perror("Setting Pixel Format"); exit(-1); }
	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_BGR24) { printf("Libv4l didn't accept format. Can't proceed.\n"); exit(EXIT_FAILURE); }
	if ((fmt.fmt.pix.width != 640) || (fmt.fmt.pix.height != 480)) { printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height); }


	struct v4l2_requestbuffers req ={0};
	req.count = 1;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (xioctl(camfd, VIDIOC_REQBUFS, &req)==-1) { perror("Requesting Buffer"); exit(-1); }


	struct v4l2_buffer buf = {0};
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;

	xioctl(camfd, VIDIOC_QUERYBUF, &buf);

	size_t bufferLen = buf.length;
	uint8_t *buffer = v4l2_mmap(NULL, bufferLen, PROT_READ | PROT_WRITE, MAP_SHARED, camfd, buf.m.offset);
	if (buffer == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }

	memset(&buf, 0, sizeof(buf));
	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory = V4L2_MEMORY_MMAP;
	buf.index = 0;
	if( xioctl(camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
	
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl(camfd, VIDIOC_STREAMON, &type)==-1) { perror("Start Capture"); exit(-1); }

	
	int threshold = 127;

	while(Run)
	{
		while(kbhit())
		{
			uint8_t ch;// = getch();
			union Ksequ sequ = {.val=0};
			uint8_t seqc=0;
			while(kbhit()) { sequ.seq[seqc++]=getch();}sequ.seq[seqc]=0;
			ch=sequ.seq[0];
			
			printf("PRESS: %d(%X)\n%llu(%08llX)\n", ch, ch, sequ.val, sequ.val);

			if (sequ.val==0x415B1B) { threshold-=5; }
			if (sequ.val==0x425B1B) { threshold+=5; }
			if (threshold<0) { threshold=0; }
			if (threshold>255) { threshold=255; }
		}

		int r;
		do 
		{
			fd_set fds;
			FD_ZERO(&fds);
			FD_SET(camfd, &fds);

			r = select(camfd + 1, &fds, NULL, NULL, &(struct timeval){.tv_sec=2, .tv_usec=0});
		} 
		while ((r == -1 && (errno = EINTR)));

		if (r == -1) { perror("Waiting for Frame"); exit(errno); }

		memset(&buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		xioctl(camfd, VIDIOC_DQBUF, &buf);


		for (unsigned int y=0; y<480; y++)
		{
			for (unsigned int x=160; x<800; x++)
			{
				struct PixelData col={0,0,0,0};//0x00000000;//[4]="\xff\x00\x00\x00";
				
				if (x-160<640) { col=*(struct PixelData *)(buffer+y*640*3+(x-160)*3); }

				uint8_t val = ((unsigned int)col.R + (unsigned int)col.G + (unsigned int)col.B)/3;


				// const unsigned int steps = 10;
				// val = (((steps*val)/256)*(255/(steps-1)));
				val = val>=(uint8_t)threshold ? 255 : 0;

				col = (!val ? (struct PixelData){0, 0, 0, 0} : (struct PixelData){col.B, col.G, col.R, 0});
				*(struct PixelData *)(fb0+y*800*4+x*4)=col;
			}
		}

		if( xioctl(camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl(camfd, VIDIOC_STREAMOFF, &type)==-1) { perror("Stop Capture"); exit(-1); }
	v4l2_munmap(buffer, bufferLen);
	v4l2_close(camfd);

	
	munmap(fb0, FB_SIZE);

	ioctl(0, KDSETMODE, KD_TEXT);

	printf("\n Done\n");

	return 0;
}
