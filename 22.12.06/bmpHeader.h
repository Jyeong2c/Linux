#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

typedef struct __attribute__((__packed__))tagBITMAPFILEhEADER{
		unsigned short bfType;
		unsigned int bfSize;
		unsigned short bfReservaed1;
		unsigned short bfReserved2;
		unsigned int bfOffBits;
}BITMAPFILEHEADER;

typedef struct{
		unsigned int biSize;
		unsigned int biWidth;
		unsigned int biHeight;
		unsigned short biPlanes;
		unsigned short biBitCount;
		unsigned int biCompression;
		unsigned int SizeImage;
		unsigned int biXpelsPerMeter;
		unsigned int biYPelsPerMeter;
		unsigned int biClrUsed;
		unsigned int biClrImportant;
}BITMAPINFOHEADER;

typedef struct{
		
		unsigned char rgbBlue;
		unsigned char rgbGreen;
		unsigned char rgbRed;
		unsigned char rgbReserved;
}RGBQUAD;

#endif
