#include "signal.h"
#include "stio.h"

void handler(int sig)
{

}

int main()
{
	struct sigaction act;
	FILE *f1;
	FILE *f2;

	f1 = fopen("/file1.txt", "w+");
	f1 = fopen("/file2.txt", "w+");
	
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0);

	while(1){
		
		sleep(5);
	}
}