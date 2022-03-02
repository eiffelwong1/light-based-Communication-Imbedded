#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>
#include "params.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include "hamming.h"

// Timestamps on laser begin and laser end
struct timespec packstart;
struct timespec packend;

// Timestamp on the last valid morse signal received
// if this is too long ago, the previous message is cleared
struct timespec prevpack;

char message[MAXLEN + 1];

// Which bit we are on
// and which character we are on
char bitcount;
int charcount;

// The current character
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

// Callback on the rising edge
// Record the time of the rising edge
// End and print the previous message
// if it had been too long since the previous pulse
// If the previous message had not been properly terminated
// with \0 and times out instead, it had been corrupted
// so we print it and reset to ensure the upcoming
// messages are not affected
void risingCall()
{
	clock_gettime(CLOCK_REALTIME, &packstart);
	unsigned long diff = timediff(packstart, prevpack);
	if(diff > TIMEOUT * 1000)
	{
		if(charcount)
		{
			printf("%s\n", message);
			charcount = 0;
		}
		bitcount = 0;
		message[0] = '\0';
		curchar = 0;
	}
	prevpack = packstart;
}

int hammingto[20], hammingfrom[8];

// Analyze the time from the previous rising edge
// and interpret the data sent
// This involves appending a bit to the current character
// and ending the current character or message
// if they are complete
void fallingCall()
{

	// Get current time
	clock_gettime(CLOCK_REALTIME, &packend);
	unsigned long packlen = timediff(packend, packstart);
	// Differences to the standard short and long
	// pulse lengths
	long sdev = STIME * 1000 - packlen;
	long ldev = LTIME * 1000 - packlen;
	sdev = sdev < 0 ? -sdev : sdev;
	ldev = ldev < 0 ? -ldev : ldev;
	char bit = sdev < ldev ? 0 : 1;
	// Alert if the pulse length is neither short nor long
	if((sdev > WARNTOL * 1000) && (ldev > WARNTOL * 1000))
	{
		fprintf(stderr, "Pulse length abnormal: %lu\n", packlen);
	}
	// Increment the counter for how many bits have
	// been transmitted in the current character
	hammingto[++bitcount] = bit;
	// Translate to ASCII if a full 14 bits
	// of hamming code had been received
	if(bitcount==14)
	{
		fromHamming(hammingto, 14, hammingfrom);
		curchar = hammingfrom[0];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[1];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[2];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[3];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[4];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[5];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[6];
		curchar = curchar << 1;
		curchar = curchar | hammingfrom[7];

		message[charcount++] = curchar;
		// Print message if string end
		// is the current character
		if(curchar=='\0')
		{
			// Print the mesasge if it is not empty
			if(charcount > 1)
			{
				printf("%s\n", message);
				message[0] = '\0';
			}
			charcount = 0;
		}
		bitcount = 0;
		curchar = 0;
	}
}

// Merge the rising and falling edge interrupts
// into one
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

	// Register the interrupt
	wiringPiISR(TAKEPIN, INT_EDGE_BOTH, changeCall);

	signal(SIGINT, signal_handler);

	sleep(240);

	return 0;
}
