

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

#define FB_SIZE (480*800*4)

// static int xioctl(int fd, int request, void *arg)
// {
// 	int r;
// 	do { r = ioctl (fd, request, arg); }
// 	while (-1 == r && EINTR == errno);
// 	return r;
// }

int main()
{
	//FILE *fbf = fopen("/dev/fb0", "wb");
	int dispfd = open("/dev/fb0", O_RDWR);
	printf("Display File Descriptor: %d\n", dispfd);

	unsigned char *fb0 = 0;//
	//unsigned char fb1[FB_SIZE];
	fb0 = mmap(0, FB_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED|MAP_POPULATE|MAP_LOCKED,dispfd,0);
	close(dispfd);
	
	// {
	// 	int camfd = open("/dev/video0", O_RDWR);
	// 	printf("Camera File Descriptor: %d\n", camfd);


	// 	printf("MMaped Address: %p\n", fb0);
	// 	struct v4l2_capability caps = {0};
	// 	if (-1 == xioctl(camfd, VIDIOC_QUERYCAP, &caps)) { perror("Querying Capabilites"); exit(-1); }


	// 	struct v4l2_format fmt = {0};
	// 	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// 	fmt.fmt.pix.width = 320;
	// 	fmt.fmt.pix.height = 240;
	// 	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	// 	fmt.fmt.pix.field = V4L2_FIELD_NONE;
	// 	if (-1 == xioctl(camfd, VIDIOC_S_FMT, &fmt)) { perror("Setting Pixel Format"); return 1; }

	// 	struct v4l2_requestbuffers req = {0};
	// 	req.count = 1;
	// 	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// 	req.memory = V4L2_MEMORY_MMAP;
	// 	if (-1 == xioctl(camfd, VIDIOC_REQBUFS, &req)) { perror("Requesting Buffer"); return 1; }

	// 	struct v4l2_buffer buf = {0};
	// 	buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	// 	buf.memory = V4L2_MEMORY_MMAP;
	// 	buf.index = 0;
	// 	if(-1 == xioctl(camfd, VIDIOC_QUERYBUF, &buf)) { perror("Querying Buffer"); return 1; }

	// 	void *buffer = mmap (NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, camfd, buf.m.offset);
	// }

	printf("Starting Main Loop...\n");
	unsigned int i = 0;
	while(1)
	{

		for (unsigned int y=0; y<480; y++)
		{
			for (unsigned int x=0; x<800; x++)
			{
				u_int32_t col=0x00000000;//[4]="\xff\x00\x00\x00";
				switch (((y+i)/60)%3)
				{
				case 0:
					col=0xff<<0; break;
				case 1:
					col=0xff<<8; break;
				case 2:
					col=0xff<<16; break;
				}
				//fwrite(col, 4, 1, fbf);
				//memcpy(fb+y*800*4+x*4, col, 4);
				*(u_int32_t *)(fb0+y*800*4+x*4)=col;
			}
		}

		//memcpy(fb0, fb1, FB_SIZE);

		//usleep(100*1000);
		i+=2;
		//fwrite("\x00", 1, 1, f);
		// fseek(fbf, 0, SEEK_SET);
		// fflush(fbf);
	}

	munmap(fb0, 480*800*4);

	//fclose(fbf);

	return 0;
}
