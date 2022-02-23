#define TIMEOUT 200 // Milliseconds for a message to be considered dropped. When this time elapses without receiving \0 or another packet, the receiver clears the buffer.
#define STIME 1000 // Microseconds for a short Morse pulse
#define LTIME 2000 // Microseconds for a long Morse pulse
#define PTIME 2500 // Microseconds for each cycle, whether short or long
#define MAXLEN 200 // Maximum length of a message
#define SENDPIN 8
#define TAKEPIN 9
