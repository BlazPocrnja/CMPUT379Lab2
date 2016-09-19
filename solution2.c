#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>

FILE *f;
jmp_buf env;
int file_number = 0;

void write_to_file(){
	while(1){
		printf("Writing to file %d...\n", file_number);
		fprintf(f, "Some text in file %d\n", file_number);
		sleep(1);
	}
}

void goto_next_file(){
	//Increments file_number, opens a new file to write to.
	file_number  += 1;

	char filename[32];
  snprintf(filename, sizeof(char) * 32, "%i.txt", file_number);

	f = fopen(filename, "w");
		//Check if opening a new file was successful.
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
}

void sigtstp_handler(int signo) {
	printf("SIGTSTP received. Going to new file...\n");
	fclose(f);
	goto_next_file();
	return;
}

void sigint_handler(int signo){
	printf("SIGINT received. Printing file names...\n");

	fclose(f);

	//Print out file names and exit.
	printf("\n");
	int i = 0;
	for(; i < file_number; i++){
		printf("%d.txt\n", i+1);
	}

	exit(0);
}

int main(int argc, char ** argv) {

	//Setting up signal handler for SIGTSTP
	struct sigaction sigtstp_act;
	
	//reset all members
        //memset(&sigtstp_act, 0, sizeof(sigtstp_act));

	//reset flags -- fixes seg fault
	sigtstp_act.sa_flags = 0;

	sigtstp_act.sa_handler = sigtstp_handler;
	sigemptyset(&sigtstp_act.sa_mask);

	sigaction(SIGTSTP, &sigtstp_act, NULL);


	//Setting up signal handler for SIGINT
	struct sigaction sigint_act;

	//reset all members
        //memset(&sigint_act, 0, sizeof(sigint_act));	
	
	//reset flags -- fixes seg fault
	sigint_act.sa_flags = 0;

	sigint_act.sa_handler = sigint_handler;
	sigemptyset(&sigint_act.sa_mask);

	sigaction(SIGINT, &sigint_act, NULL);

	goto_next_file();
	write_to_file();

	return 0;
}
