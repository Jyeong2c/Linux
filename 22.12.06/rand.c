#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
//	srand(10);
	srand((unsigned int)time(NULL));
	printf("A : %d\n", rand());
	printf("A : %d\n", rand());
	printf("A : %d\n", rand());
	printf("A : %d\n", rand());
	printf("A : %d\n", rand());
	return 0;
}
