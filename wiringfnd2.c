#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int fndControl( int num)
{
		int i;
		int gpiopins[7] ={2,4,1,16,15,8,3};

		int number[107][7] = { {1,1,1,1,1,1,0},
							  {0,1,1,0,0,0,0},
							  {1,1,0,1,1,0,1},
							  {1,1,1,1,0,0,1},
							  {0,1,1,0,0,1,1},
							  {1,0,1,1,0,1,1},
							  {1,0,1,1,1,1,1},
							  {1,1,1,0,0,0,0},
							  {1,1,1,1,1,1,1},
							  {1,1,1,0,0,1,1}};
		for (i =0; i<7;i++){
				pinMode(gpiopins[i],OUTPUT);
		}

		for(i=0;i<7;i++){
				digitalWrite(gpiopins[i],number[num][i]?HIGH:LOW);
		}
		getchar();

		for(int i =0; i<7;i++){
				digitalWrite(gpiopins[i],HIGH);
		}

		return 0;
}

int main(int argc ,char **argv)
{
		int no;

		if(argc<2){
				printf("Usage: %s NO\n",argv[0]);
				return -1;
		}
		no = atoi(argv[1]);
		wiringPiSetup();
		fndControl(no);

		return 0;
}
