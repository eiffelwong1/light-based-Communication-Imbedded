#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include "params.h"
#include <stdlib.h>
#include <time.h>



void signal_handler(int signum) {
	printf("\nProgram exits by SIGINT.\n");
	exit(0);
}

int main()
{
	if(!wiringPiSetup() == -1)
	{
		printf("Flash: wiringPi setup failed\n");
	}
	pinMode(SENDPIN, OUTPUT);

	struct timespec sleep_time;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 10;
	struct timespec remaining_time;

	register unsigned int i, k;

	// int freq, micro;
	// printf("Enter frequency: ");
	// scanf("%d", &freq);
	// printf("%d\n", freq);
	// micro = 1000000 / freq / 2;
	// printf("%d\n", micro);
	while(0x2e)
	{
		digitalWrite(SENDPIN, HIGH);
		// usleep(micro);
		// nanosleep(&sleep_time, &remaining_time);
		for(i=0;i<10000;i++)
		{
			k++;
		}
		digitalWrite(SENDPIN, LOW);
		// usleep(micro);
		// nanosleep(&sleep_time, &remaining_time);
		for(i=0;i<10000;i++)
		{
			k++;
		}
		digitalWrite(SENDPIN, LOW);
	}
}
