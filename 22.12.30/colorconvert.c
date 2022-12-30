#include <stdio.h>

typedef unsigned char ubyte;

extern int readBmp(char *filename, ubyte **pData, int *cols, int *rows, int *color);

void cvtBGR2Gray(ubyte *dest , ubyte *src, int n)
{

	for(int i =0; i<n; i++){
		int b = *scr++;
		int g = *scr++;
		int r = *scr++;

		int y = (r*77)+(g*151)+(b*28);
		*dest++ =(y>>8);
		*dest++ =(y>>8);
		*dest++ =(y>>8);
	}
}

int main(int argc , char **argv)
{
	int cols, rows,color =24;
	ubyte r, g, b, a =255;
	ubyte * pData, *pBmpData, *pImageData, *pFbMap;
	struct fb_var_screeninfo vinfo;

	pData = (ubyte*)malloc(vinfo.xres *vinfo.yres * sizeof(ubyte) *color/8);
	pImageData = (ubyte*)malloc(vinfo.xres *vinfo.yres * sizeof(ubyte)*color/8);

	if(readBmp(argv[1],&pImageData, &cols, &rows,&color)<0){
		perror("readBmp()");
		return -1;
	}
	cvtBGR2Gray(pData, pImageData,vinfo.xres*vinfo.yres);
