#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>
#include "params.h"
#include <string.h>
#include <unistd.h>
#include "hamming.h"

// Send an ASCII character
void sendChar(char letter)
{
	// Manually written out for better performance
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
		// Determine the length of this laser pulse
		// Account for the measureed overhead of the usleep function
		sleeptime = hammingfrom[i] ? LTIME : STIME;
		sleeptime = sleeptime > OVERHEAD ? sleeptime - OVERHEAD : sleeptime;
		// Start laser
		digitalWrite(SENDPIN, HIGH);
		usleep(sleeptime);
		// Stop laser
		digitalWrite(SENDPIN, LOW);
		// Wait with the laser stopped for a falling
		// edge interrupt to be triggered
		// on the receiving side
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
	// Send a few 0's because
	// the first pulses appear to be
	// exceptionally inaccurate.
	// This "warms up" the system
	sendChar('\0');
	sendChar('\0');
	sendChar('\0');
	sendChar('\0');
	pinMode(SENDPIN, OUTPUT);
	// Keep reading messages until reading quit
	printf("Enter message\n");
	scanf("%s", message);
	while(strcmp(message, quit))
	{
		for(i=0;message[i]!='\0';i++)
		{
			sendChar(message[i]);
		}
		sendChar('\0');
		// Wait briefly to trigger a timeout
		// on the receiving side.
		// This is done so that a mis-alignment caused
		// by completely missing a laser pulse
		// can be corrected at the end of each word
		usleep(PTIME << 3);
		scanf("%s", message);
	}
	printf("Quit message received, exiting\n");
	return 0;
}
