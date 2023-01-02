#include <stdio.h>
#include <stdlib.h>
#include "bmpHeader.h"

typedef unsigned char ubyte;

int main(int argc, char ** argv)
{
	FILE *file;
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	ubyte *inimg, *outimg;
	int x,imageSize;
	RGBQUAD *pal;

	if(argc != 3){
		fprintf(stderr, "usage : %s input.bmp outimg.bmp ",argv[0]);
		return -1;
	}

	if((file = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, " Error : Failed to open file...\n");
		return -1;
	}

	fread(&bmpFile, sizeof(BITMAPFILEHEADER),1,file);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER),1,file);

	if(bmpInfo.biBitCount != 24){
		perror("This image file doesn't supports 24bit color\n");
		fclose(file);
		return -1;
	}

	int elemSize = bmpInfo.biBitCount/8;
	int size = bmpInfo.biWidth*elemSize;
	imageSize = size*bmpInfo.biHeight;

	inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
	

	fread(inimg, sizeof(ubyte),imageSize,file);
	
	fclose(file);
	
	for(x= 0;x<bmpInfo.biHeight*size; x+=elemSize){
		//	ubyte b = inimg[x*size+0];
		//	ubyte g = inimg[x*size+1];
		//	ubyte r = inimg[x*size+2];

			*(outimg+x+0) = *(inimg+x+0);
			*(outimg+x+1) = *(inimg+x+1);
			*(outimg+x+2) = *(inimg+x+2);
		}
	if((file =fopen(argv[2],"wb"))==NULL){
		fprintf(stderr,"Error : Failed to open file...\n");
		return -1;
	}

	pal = (RGBQUAD*)malloc(sizeof(RGBQUAD)*256);
	for(x = 0; x<256; x++){
		pal[x].rgbBlue = pal[x].rgbGreen = pal[x].rgbRed = x;
		pal[x].rgbReserved = 0;
	}

	bmpFile.bfOffBits = sizeof(BITMAPFILEHEADER)+ sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256;
	bmpFile.bfSize = bmpFile.bfOffBits + bmpInfo.SizeImage;

	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER),1,file);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER),1,file);
	fwrite(pal,sizeof(RGBQUAD),256,file);
	fwrite(outimg,sizeof(ubyte),imageSize,file);
	
	fclose(file);

	free(outimg);
	free(inimg);

	return 0;
}
