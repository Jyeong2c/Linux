#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <time.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

// 오름차순으로 정렬할 때 사용하는 비교함수
int static compare(const void* first, const void* second)
{
    if (*(int*)first > *(int*)second)
        return 1;
    else if (*(int*)first < *(int*)second)
        return -1;
    else
        return 0;

}

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    ubyte *inimg, *outimg, *padimg;
    int i, x, y, z, cnt, elemSize, imageSize, size;
    srand((unsigned int)time(NULL)); 	// seed값으로 현재시간 부여

    if(argc != 4) {
        fprintf(stderr, "usage : %s count input.bmp output.bmp\n", argv[0]);
        return -1;
    }
    
    /***** read bmp *****/ 
    if((fp=fopen(argv[2], "rb")) == NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    /* 트루 컬러를 지원하지 않으면 표시할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
    
    elemSize = bmpInfoHeader.biBitCount/8;
    size = bmpInfoHeader.biWidth*elemSize;
    imageSize = size * bmpInfoHeader.biHeight;

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);
    
    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize); 
    fread(inimg, sizeof(ubyte), imageSize, fp); 
    //fseek(fp, -1*sizeof(ubyte)*imageSize, SEEK_CUR);
    //fread(outimg, sizeof(ubyte), imageSize, fp); 
    
    fclose(fp);
    
    /* add White noise */
    cnt = atoi(argv[1]);
    for(i = 0; i < cnt; i++) { 
        int pos = rand( ) % (bmpInfoHeader.biWidth * bmpInfoHeader.biHeight);
        int value = rand( ) & 0b11111111;
        for(z = 0; z < elemSize; z++) {
            int tmp = inimg[pos*elemSize+z] + value;
            inimg[pos*elemSize+z] = LIMIT_UBYTE(tmp);
        }
     }         

    int padSize = (bmpInfoHeader.biWidth + 2) * elemSize;
    int addSize = (padSize + bmpInfoHeader.biHeight)*2;
    padimg = (ubyte*)malloc(sizeof(ubyte)*(imageSize + addSize));

    /* make padding image */
    memset(padimg, 0, (sizeof(ubyte)*imageSize + addSize));

    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                padimg[(x+elemSize)+(y+1)*padSize+z]=inimg[x+y*size+z];
            }
        }
    }

    for(y = 0; y < bmpInfoHeader.biHeight; y++) { 
        for(z = 0; z < elemSize; z++) {
            padimg[0+(y+1)*padSize+z]=inimg[0+y*size+z];
            padimg[padSize-elemSize+(y+1)*padSize+z]=inimg[size-elemSize+y*size+z];
        }
    }

    for(x = 0; x < bmpInfoHeader.biWidth*elemSize; x++) { 
        padimg[elemSize+x]=inimg[x];
        padimg[elemSize+x+(bmpInfoHeader.biHeight+1)*padSize]=inimg[x+(bmpInfoHeader.biHeight-1)*size];
    }

    for(z = 0; z < elemSize; z++) {
       padimg[z]=inimg[z];
       padimg[padSize-elemSize+z]=inimg[size-elemSize+z];
       padimg[(bmpInfoHeader.biHeight+1)*padSize+z]=inimg[(bmpInfoHeader.biHeight-1)*size+z];
       padimg[(bmpInfoHeader.biHeight+1)*padSize+padSize-elemSize+z]=inimg[(bmpInfoHeader.biHeight-1)*size+size-elemSize+z];
    }

    memset(outimg, 0, sizeof(ubyte)*imageSize);
    int arr[9];
    for(y = 1; y < bmpInfoHeader.biHeight + 1; y++) { 
        for(x = elemSize; x < padSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                //float sum = 0.0;
                int cnt = 0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        arr[cnt++] = padimg[(x+i*elemSize)+(y+j)*padSize+z];
                    }
                }
                qsort(arr, sizeof(arr) / sizeof(int), sizeof(int), compare);
                outimg[(x-elemSize)+(y-1)*size+z] = LIMIT_UBYTE(arr[4]);
            }
        }
    }         
     
    /***** write bmp *****/ 
    if((fp=fopen(argv[3], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }
    
    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    fwrite(outimg, sizeof(unsigned char), imageSize, fp);
    
    fclose(fp); 

    if((fp=fopen("white.bmp", "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    fwrite(inimg, sizeof(unsigned char), imageSize, fp);
    
    fclose(fp); 

    free(inimg); 
    free(outimg); 
    
    return 0;
}

