#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>



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
	int freq, micro;
	printf("Enter frequency: ");
	scanf("%d", freq);
	micro = 1000000 / freq / 2;
	while(true)
	{
		digitalWrite(SENDPIN, HIGH);
		usleep(micro);
		digitalWrite(SENDPIN, LOW);
		usleep(micro);
	}
}
