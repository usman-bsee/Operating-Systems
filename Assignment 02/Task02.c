#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
int main(void)
{
	int pipefd1[2], pipefd2[2];
	char *Ppin = "I am Parent\0", *Cpin = "I am Child\0";
	int buffer[5] = {}, iterations = 5;
	if(pipe(pipefd1) ==-1 || pipe(pipefd2) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	struct timeval tv; struct timezone tz;
	gettimeofday(&tv, &tz);
	pid_t pid= fork();
	if(pid == 0)
	{
		for(int i=0;i<iterations;i++)
		{
			printf("\nChild Iteration: %d\t", i);
			read(pipefd2[0], buffer, 11);
			printf("Child Read: %s\t", buffer);
			sleep(1);
			write(pipefd1[1], Cpin, 10);
			printf("\tChild Write: %s\n", Cpin);
			sleep(1);
		}
		struct timeval tv1; struct timezone tz1;
		gettimeofday(&tv1, &tz1);
		float finalTime = tv1.tv_usec - tv.tv_usec;
		printf("\nChild Final overhead time(Micro-seconds): %f\n", finalTime/iterations);
	}
	if(pid > 0)
	{
		for(int i=0; i<iterations; i++)
		{
			printf("Parent Iteration: %d\t", i);
			write(pipefd2[1], Ppin, 11);
			printf("Parent Write: %s\t", Ppin);
			sleep(1);
			read(pipefd1[0], buffer, 10);
			printf("Parent Read: %s\n", buffer);
			sleep(1);
		}
		struct timeval tv1; struct timezone tz1;
		gettimeofday(&tv1, &tz1);
		float finalTime = tv1.tv_usec - tv.tv_usec;
		printf("\nParent Final overhead time(Micro-seconds): %f\n", finalTime/iterations);
	}
	return 0;
}


