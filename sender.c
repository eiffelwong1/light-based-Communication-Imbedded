#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>
#include "params.h"
#include <string.h>
#include <unistd.h>

void sendChar(char letter)
{
	// Send a character as 8 short or long pulses
	char i;
	unsigned int sleeptime;
	for(i=0;i<8;i++)
	{
		// TODO: investigate how long it takes for digitalWrite to execute. take this into account
		sleeptime = (letter >> (7-i)) & 1 ? LTIME : STIME;
		sleeptime = sleeptime > OVERHEAD ? sleeptime - OVERHEAD : sleeptime;
		digitalWrite(SENDPIN, HIGH);
		usleep(sleeptime);
		digitalWrite(SENDPIN, LOW);
		usleep(PTIME - sleeptime);
	}
}

int main()
{
	char message[MAXLEN + 1];
	const char* quit = "quit\0";
	int i;
	if(wiringPiSetup() == -1)
	{
		printf("Sender: wiringpi setup failed\n");
		return 1;
	}
	sendChar('\0');
	sendChar('\0');
	sendChar('\0');
	sendChar('\0');
	pinMode(SENDPIN, OUTPUT);
	printf("Enter message\n");
	scanf("%s", message);
	while(strcmp(message, quit))
	{
		for(i=0;message[i]!='\0';i++)
		{
			sendChar(message[i]);
		}
		sendChar('\0');
		sendChar('\0');
		scanf("%s", message);
	}
	printf("Quit message received, exiting\n");
	return 0;
}
