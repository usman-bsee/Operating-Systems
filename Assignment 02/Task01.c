#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

int main()
{
	struct timeval tv; struct timezone tz;
	gettimeofday(&tv, &tz);
	printf("Starting Time(seconds): %lu\t", tv.tv_sec);
	printf("Starting Time(Micro-seconds): %d\n", tv.tv_usec);
	for(int i=0;i<10000;i++)
	{
		char *temp = (char*) malloc(sizeof(char));
		free(temp);
	}
	
	struct timeval tv1; struct timezone tz1;
	gettimeofday(&tv1, &tz1);
	//time_t startTime = time(NULL);
	printf("Ending Time(seconds): %lu\t", tv1.tv_sec);
	printf("Ending Time(Micro-seconds): %d\n", tv1.tv_usec);
	int finalTime = tv1.tv_usec - tv.tv_usec;
	printf("\nFinal overhead time(Micro-seconds): %d\n", finalTime);

	return 0;
}
