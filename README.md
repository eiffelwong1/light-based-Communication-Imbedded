# light-based-Communication-Embedded
Light-based communication that utilizes lasers. Written in C

## Organization
**sender.c**: The transmitter, which takes inputs from stdin and transmits the messages via the laser

**receiver.c**: The receiver, which monitors the light sensor, translates the pulses to ASCII messages, and outputs the messages to stdout

**params.h**: Header file that defines shared configurations between the sender and receiver

**Makefile**: Cmake directions

**hamming.h**: Functions that translate to and from hamming code

**hamming.c**: Test program that validates the correctness of the hamming code functions

**flash.c**: Test program that flashes the laser on and off at a nominal frequency taken from stdin. This program helps measure the overhead of the sleep functions

## To run
```bash
make
./sender
./receiver
```

Note that two terminal/ssh sessions are needed to run the sender and receiver simultaneously
