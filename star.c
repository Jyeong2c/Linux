#include <math.h>

static int DrawStart(int fd, int x, int y, unsigned short color)
{
    struct fb_var_screeninfo vinfo;
    unsigned short *pfb;
    double alpha = (2 * M_PI) / 10; 
    int x, y, offset, radius = 12;

    /* 현재 프레임버퍼에 대한 고정된 화면 정보를 얻어온다. */
    if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }
 
    pfb = (unsigned short *)mmap(0, vinfo.xres_virtual*vinfo.yres_virtual*2, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);

	for(int i = 11; i != 0; i--) {
        double r = radius*(i % 2 + 1)/2;
        double omega = alpha * i;
        *(pfb + r * Math.sin(omega) + x 
              + (r * Math.cos(omega) + y) * vinfo.xres_virtual) 
    			= color;
    }

	munmap(pfb, vinfo.xres*vinfo.yres*2);

	return 0;
}

