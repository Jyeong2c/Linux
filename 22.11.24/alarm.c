#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

void signalHandler(int);


int main()
{
		signal(SIGINT,SIG_IGN);
		signal(SIGALRM, signalHandler);
		alarm(2);
		while(1)pause();
		return 0;
}

void signalHandler(int no)
{
		if(no == SIGALRM){
				printf("%s\n", strsignal(no));
		}
		alarm(2);
}
