#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>
#include "params.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

// Timestamps on laser begin and laser end
struct timespec packstart;
struct timespec packend;

// Timestamp on the last valid morse signal received
// if this is too long ago, the previous message is cleared
struct timespec prevpack;

char message[MAXLEN + 1];

// Which bit we are on
char bitcount;
int charcount;

char curchar;

unsigned long billion = 1000000000;

void signal_handler(int signum) {
	printf("\nProgram exits by SIGINT.\n");
	exit(0);
}

// Time difference in nanoseconds
unsigned long timediff(struct timespec a, struct timespec b)
{
	return billion * (a.tv_sec - b.tv_sec) + a.tv_nsec - b.tv_nsec;
}

void risingCall()
{
	clock_gettime(CLOCK_REALTIME, &packstart);
	unsigned long diff = timediff(packstart, prevpack);
	if(diff > TIMEOUT * 1000000)
	{
		bitcount = 0;
		charcount = 0;
		message[0] = '\0';
		curchar = 0;
	}
	prevpack = packstart;
}

void fallingCall()
{
	clock_gettime(CLOCK_REALTIME, &packend);
	unsigned long packlen = timediff(packend, packstart);
	long sdev = STIME * 1000 - packlen;
	long ldev = LTIME * 1000 - packlen;
	sdev = sdev < 0 ? -sdev : sdev;
	ldev = ldev < 0 ? -ldev : ldev;
	char bit = sdev < ldev ? 0 : 1;
	if((sdev > WARNTOL * 1000) && (ldev > WARNTOL * 1000))
	{
		printf("Pulse length abnormal: %lu\n", packlen);
	}
	bitcount++;
	curchar = (curchar << 1) | bit;
	if(bitcount==8)
	{
		message[charcount++] = curchar;
		if(curchar=='\0')
		{
			printf("%s\n", message);
			charcount = 0;
			message[0] = '\0';
		}
		bitcount = 0;
		curchar = 0;
	}
}

void changeCall()
{
	if(digitalRead(TAKEPIN))
	{
		risingCall();
	}
	else
	{
		fallingCall();
	}
}

int main()
{
	if(wiringPiSetup() == -1)
	{
		printf("Receiver: wiringPi setup failed\n");
	}
	pinMode(TAKEPIN, INPUT);

	// Set bit counter to 0
	bitcount = 0;
	charcount = 0;
	message[0] = '\0';
	curchar = 0;

	wiringPiISR(TAKEPIN, INT_EDGE_BOTH, changeCall);

	signal(SIGINT, signal_handler);

	sleep(240);

	return 0;
}
