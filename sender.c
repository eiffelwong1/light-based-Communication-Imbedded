#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>
#include "params.h"
#include <string.h>
#include <unistd.h>
#include "hamming.h"

void sendChar(char letter)
{
	int hammingto[8], hammingfrom[20];
	hammingto[0] = (letter >> 7) & 1;
	hammingto[1] = (letter >> 6) & 1;
	hammingto[2] = (letter >> 5) & 1;
	hammingto[3] = (letter >> 4) & 1;
	hammingto[4] = (letter >> 3) & 1;
	hammingto[5] = (letter >> 2) & 1;
	hammingto[6] = (letter >> 1) & 1;
	hammingto[7] = letter & 1;
	toHamming(hammingto, 8, hammingfrom);

	// Send a character as 8 short or long pulses
	char i;
	unsigned int sleeptime;
	for(i=1;i<15;i++)
	{
		// TODO: investigate how long it takes for digitalWrite to execute. take this into account
		sleeptime = hammingfrom[i] ? LTIME : STIME;
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
		usleep(PTIME << 3);
		scanf("%s", message);
	}
	printf("Quit message received, exiting\n");
	return 0;
}
