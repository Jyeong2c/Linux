#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)
//0보다 작은값은 0 으로 255보다 큰 값은 255으로하는 이미지 경계 검사

typedef unsigned char ubyte;

int main(int argc, char** argv)
{
    FILE* fp;
    BITMAPFILEHEADER bmpHeader;         //BITMAPFILEHEADER 구조체를 사용하기 위해 선언    
    BITMAPINFOHEADER bmpInfoHeader;     //BITMAPINFOHEADER 구조체를 사용하기 위해 선언
    ubyte* inimg, * outimg, * padimg;       
    int x, y, z, imageSize, paddingSize;
		
		/*인자 값이 3개가 아니면 예외처리*/
    if (argc != 3) {
        fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
        return -1;
    }

    /* 파일 오픈 시 첫 번째 NULL값이면 예외처리*/
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
		/* BITMAPINFOHEADER 구조체의 데이터 */
		/* 읽어들일 파일, 데이터 하나의 크기 , 데이터의 개수 , 대상 파일 스트림*/
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

		/*24비트 이미지가 아니면 예외 처리*/
    if (bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);
    printf("paddingSize :%d\n", paddingSize);
    printf("padimg : %d\n", imageSize + paddingSize);
		

		/*픽셀당 비트 수 24/8 =3 (elemSize) b,g,r*/
    int elemSize = bmpInfoHeader.biBitCount / 8;
		/* 이미지 폭에 b,g,r 값을 곱한 값을 size에 재정의*/
    int size = bmpInfoHeader.biWidth * elemSize;
		/*이미지 높이에 size를 곱한 값을 재정의(이미지 크기) */
    imageSize = size * bmpInfoHeader.biHeight;
		/*패딩 사이즈를 설정*/
    paddingSize = ((bmpInfoHeader.biWidth + bmpInfoHeader.biHeight + 2) * elemSize) * 2;

		/*인 이미지 , 아웃 이미지, 패딩 이미지 메모리 할당 */
    inimg = (ubyte*)malloc(sizeof(ubyte) * imageSize);
    outimg = (ubyte*)malloc(sizeof(ubyte) * imageSize);
    padimg = (ubyte*)malloc(sizeof(ubyte) * imageSize + paddingSize);

		/*inimg를 읽는다*/    
		fread(inimg, sizeof(ubyte), imageSize, fp);
    fclose(fp);  //원하는 값을 읽어오고 파일을 닫는다 

		/*평균값 필터를 사용하기 위해 마스크 설정*/
    float kernel[3][3] = { {1 / 9.0, 1 / 9.0, 1 / 9.0},
                           {1 / 9.0, 1 / 9.0, 1 / 9.0},
                           {1 / 9.0, 1 / 9.0, 1 / 9.0} };
	
		/*패딩이미지의 초기값을 0으로 설정하고 사이즈를 설정 해 준다*/
    memset(padimg, 0, sizeof(ubyte) * imageSize + paddingSize);

		/*패딩 이미지가 0부터 시작하고 biHeight에서 +1 해줘야지 패딩까지 채워진다 */
    for (y = 0; y < bmpInfoHeader.biHeight + 1; y++) {
        for (x = 0; x < (bmpInfoHeader.biWidth + 1) * elemSize; x += elemSize) {
            for (z = 0; z < elemSize; z++) {
                float sum = 0.0;
                for (int i = -1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        sum += kernel[i + 1][j + 1] * inimg[(x + i * elemSize)
																															 + (y + j) * size + z];
                    }
                }
                outimg[x + y * size + z] = LIMIT_UBYTE(sum);
            }
        }
    }

		/*파일을 오픈할 때 2번째 인자가 NULL이면 예외 처리*/
    /***** write bmp *****/
    if ((fp = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "Error : Failed to open file...₩n");
        return -1;
    }
		/*변경 된 값 쓰기*/
    /* BITMAPFILEHEADER 구조체의 데이터 */
		/* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(outimg, sizeof(ubyte), imageSize ,fp);

    fclose(fp);

		/*메모리 할당 해제*/
    free(inimg);
    free(outimg);
    free(padimg);

    return 0;
}
