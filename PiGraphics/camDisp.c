
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <asm-generic/mman.h>
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


#define CAMWIDTH 640
#define FBWIDTH 800
#define HEIGHT 480

#define FB_SIZE (HEIGHT*FBWIDTH*4)

#define ARRACC(b,x,y,w,s,t) *(t)((b)+(y)*(w)*(s)+(x)*(s))
#define FBACC(b,x,y) (ARRACC((b), (x), (y), FBWIDTH, 4, u_int32_t *))
#define CAMACC(b,x,y) (ARRACC((b), (x), (y), CAMWIDTH, 3, u_int32_t *)&0xFFFFFF)
#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

// struct PixelData
// {
// 	uint8_t B;
// 	uint8_t G;
// 	uint8_t R;
// 	uint8_t Empty;
// };

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

struct FloodFillLine
{
	unsigned int X, Y;
};

uint8_t Run = 1;

void InteruptHandler(int arg) { reset_terminal_mode(); Run=0; }

int main()
{
	signal(SIGINT, InteruptHandler); signal(SIGKILL, InteruptHandler);

	//ioctl(0, KDSETMODE, KD_GRAPHICS);
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
	fmt.fmt.pix.width       = CAMWIDTH;
	fmt.fmt.pix.height      = HEIGHT;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24; //V4L2_PIX_FMT_RGB24
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	if (xioctl(camfd, VIDIOC_S_FMT, &fmt)==-1) { perror("Setting Pixel Format"); exit(-1); }
	if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_BGR24) { printf("Libv4l didn't accept format. Can't proceed.\n"); exit(EXIT_FAILURE); }
	if ((fmt.fmt.pix.width != CAMWIDTH) || (fmt.fmt.pix.height != HEIGHT)) { printf("Warning: driver is sending image at %dx%d\n", fmt.fmt.pix.width, fmt.fmt.pix.height); }


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

		u_int32_t camFrame[HEIGHT][CAMWIDTH];
		for (unsigned int y=0; y<HEIGHT; y++)
		{
			for (unsigned int x=0; x<CAMWIDTH; x++)
			{
				//struct PixelData col={0,0,0,0};//0x00000000;//[4]="\xff\x00\x00\x00";
				
				uint32_t col = CAMACC(buffer, x, y);

				uint8_t val = (((col>>24)&0xFF) + ((col>>16)&0xFF) + (col&0xFF))/3;

				if (val<(uint8_t)threshold) { col = 0x00000000; }
				
				camFrame[y][x] = col;
			}
		}

		u_int32_t regionmap[HEIGHT][CAMWIDTH];
		memset(regionmap, 0, CAMWIDTH*HEIGHT*sizeof(u_int32_t));

		u_int32_t maxregion;
		u_int32_t avgX=0, avgY=0, avgC=0;

		{		
			u_int32_t region=0;
			
			maxregion=0;
			u_int32_t maxregionarea = 0;
			
			struct FloodFillLine lineStack[HEIGHT]; u_int32_t lineStackP=-1;

			const unsigned int crop=40;
			for (unsigned int y=crop; y<HEIGHT-crop; y++)
			{
				for (unsigned int x=crop; x<CAMWIDTH-crop; x++)
				{
					if (camFrame[y][x]==0 && regionmap[y][x]==0)
					{ 
						
						//Flood Fill
						region++;
						u_int32_t regionarea = 0;
						
						lineStack[++lineStackP] = (struct FloodFillLine){x,y};
						
						unsigned int yf=0, xf=0;
						
						//Handle every line that is part of the region
						while(lineStackP<-1)
						{
							//Pop off of stack
							xf=lineStack[lineStackP].X;
							yf=lineStack[lineStackP].Y;
							lineStackP--;
							
							
							//Check if line is incomplete
							if (regionmap[yf][xf]==0)
							{
								char ue=1, le=1; 
								
								//Move to the beginning of the line
								while(xf>crop && camFrame[yf][xf-1]==0) { xf--; }
								
								//Loop forward through the line
								while(xf<CAMWIDTH-crop && camFrame[yf][xf]==0)
								{
									regionmap[yf][xf] = region;
									//*(u_int32_t *)(fb0+yf*width*4+xf*4) = 0xFF00FFFF;
									regionarea++;
									
									//Check if line above or below is unaccounted for. If so, add it to the stack.
									//Only do this at the beginning of a line or after a non-region above/below 
									if (yf>crop && camFrame[yf-1][xf]==0) { if (ue && !regionmap[yf-1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf-1}; ue=0; } }
									else { ue = 1; }
									if (yf<HEIGHT-crop-1 && camFrame[yf+1][xf]==0) { if (le && !regionmap[yf+1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf+1}; le=0; } }
									else { le = 1; }
									
									xf++; 
								}
							}
							
						}
						
						if (regionarea > maxregionarea) { maxregionarea=regionarea; maxregion=region; }
						
					}
				}
			}

			for (int y=0; y<HEIGHT; y++)
			{
				for (int x=0; x<CAMWIDTH; x++)
				{
					if (regionmap[y][x] == maxregion) { avgX+=x; avgY+=y; avgC++; }
				}
			}
			avgX=avgX/avgC;
			avgY=avgY/avgC;
		}

		for (unsigned int y=0; y<HEIGHT; y++)
		{
			for (unsigned int x=0; x<FBWIDTH; x++)
			{

				uint32_t col = 0;
				if (x>=160)
				{
					if (maxregion && regionmap[y][x-160]==maxregion) { col = 0x0000FF00; }
					else if (regionmap[y][x-160]) { col = 0x0000FFFF; }
					else { col = camFrame[y][x-160]; } 
				}
				else { col = 0x00000000; }

	
				if (abs((int)(x-160)-(int)avgX) <= 5 && abs((int)y-(int)avgY) <= 5)
				{
					col = 0x00FF0000;
				}

				FBACC(fb0, x, y) = col;
			}
		}

		if( xioctl(camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl(camfd, VIDIOC_STREAMOFF, &type)==-1) { perror("Stop Capture"); exit(-1); }
	v4l2_munmap(buffer, bufferLen);
	v4l2_close(camfd);

	
	munmap(fb0, FB_SIZE);

	//ioctl(0, KDSETMODE, KD_TEXT);

	printf("\n Done\n");

	return 0;
}
