
#include <fcntl.h>
#include <sys/mman.h>
//#include <sys/ioctl.h>

#include <linux/videodev2.h>
#include <libv4l2.h>

#include "camera.h"

static int xioctl(int fd, int request, void *arg)
{
	int r;

	do { r = v4l2_ioctl(fd, request, arg); } 
	while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

	return r;
}

void InitCamera(int *camfd, u8 **buffer)
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

	// memset(&buf, 0, sizeof(buf));
	// buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// buf.memory = V4L2_MEMORY_MMAP;
	// buf.index = 0;
	// if( xioctl(*camfd, VIDIOC_QBUF, &buf)==-1) { perror("Queue Buffer"); exit(-1); }
	
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (xioctl(*camfd, VIDIOC_STREAMON, &type)==-1) { perror("Start Capture"); exit(-1); }
}

void CloseCamera(int camfd, u8 **buffer)
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

void WaitForFrame(int camfd)
{
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
}


struct FloodFillLine
{
	u32 X, Y;
};

void EyeTracking(u8 *cambuff, int *eyeX, int *eyeY, int threshold, u32 *fb)
{
	u32 camFrame[CAMHEIGHT][CAMWIDTH];

	//Preform initial treshold gating
	for (u32 y=0; y<CAMHEIGHT; y++)
	{
		for (u32 x=0; x<CAMWIDTH; x++)
		{
			//struct PixelData col={0,0,0,0};//0x00000000;//[4]="\xff\x00\x00\x00";
			
			u32 col = CAMACC(cambuff, x, y);

			u8 val = (((col>>24)&0xFF) + ((col>>16)&0xFF) + (col&0xFF))/3;

			if (val<(u8)threshold) { col = 0x00000000; }
			
			camFrame[y][x] = col;
		}
	}

	u32 regionmap[CAMHEIGHT][CAMWIDTH];
	memset(regionmap, 0, CAMWIDTH*CAMHEIGHT*sizeof(u32));

	u32 maxregion;
	u32 avgX=0, avgY=0, avgC=0;

	//Main algorithm
	{		
		u32 region=0;
		
		maxregion=0;
		u32 maxregionarea = 0;
		
		struct FloodFillLine lineStack[CAMHEIGHT]; u32 lineStackP=-1;

		const u32 crop=0;
		//For every Pixel in the Frame (except crop region)
		for (u32 y=crop; y<CAMHEIGHT-crop; y++)
		{
			for (u32 x=crop; x<CAMWIDTH-crop; x++)
			{
				//if black and not part of region
				if (camFrame[y][x]==0 && regionmap[y][x]==0)
				{ 
					
					//Flood Fill
					region++;
					u32 regionarea = 0;
					
					lineStack[++lineStackP] = (struct FloodFillLine){x,y};
					
					u32 yf=0, xf=0;
					
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
								//*(u32 *)(fb0+yf*width*4+xf*4) = 0xFF00FFFF;
								regionarea++;
								
								//Check if line above or below is unaccounted for. If so, add it to the stack.
								//Only do this at the beginning of a line or after a non-region above/below 
								if (yf>crop && camFrame[yf-1][xf]==0) { if (ue && !regionmap[yf-1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf-1}; ue=0; } }
								else { ue = 1; }
								if (yf<CAMHEIGHT-crop-1 && camFrame[yf+1][xf]==0) { if (le && !regionmap[yf+1][xf]) { lineStack[++lineStackP] = (struct FloodFillLine){xf,yf+1}; le=0; } }
								else { le = 1; }
								
								xf++; 
							}
						}
						
					}
					
					if (regionarea > maxregionarea) { maxregionarea=regionarea; maxregion=region; }
					
				}
			}
		}

		//Calculate Centerpoint of maxregion
		for (u32 y=0; y<CAMHEIGHT; y++)
		{
			for (u32 x=0; x<CAMWIDTH; x++)
			{
				if (regionmap[y][x] == maxregion) { avgX+=x; avgY+=y; avgC++; }
			}
		}
		avgX=avgX/avgC;
		avgY=avgY/avgC;
	}

	//Display to Frame Buffer
	if (fb)
	{
		for (u32 y=0; y<CAMHEIGHT; y++)
		{
			for (u32 x=0; x<CAMWIDTH; x++)
			{
				fb[y*CAMWIDTH+x]=CAMACC(cambuff, x, y);

				//u32 col = 0;
				Pixel col = (Pixel){.R=0x00,.G=0x00,.B=0x00,.A=0x00};
				if (maxregion && regionmap[y][x]==maxregion) { col = (Pixel){.R=0x00,.G=0xFF,.B=0x00,.A=0x8F}; }
				else if (regionmap[y][x]) { col = (Pixel){.R=0x00,.G=0xFF,.B=0xFF,.A=0x3F}; }
				//else { col = WordToPixel(CAMACC(cambuff, x, y)); col.A=0xFF; }


				if (abs((int)(x)-(int)avgX) <= 5 && abs((int)y-(int)avgY) <= 5)
				{
					col = (Pixel){.R=0xFF,.G=0x00,.B=0x00,.A=0xFF};
				}

				SetPixel(fb, CAMWIDTH, x, y, col);
			}
		}
	}

	*eyeX=avgX;
	*eyeY=avgX;

}

