#define TIMEOUT 1200 // Microseconds for a message to be considered dropped. When this time elapses without receiving \0 or another packet, the receiver clears the buffer.
#define STIME 100 // Microseconds for a short Morse pulse
#define LTIME 200 // Microseconds for a long Morse pulse
#define PTIME 250 // Microseconds for each cycle, whether short or long
#define WARNTOL 200 // Microseconds by which the length of a pulse is allowed to deviate from one of STIME and LTIME before a warning is generated 
#define MAXLEN 200 // Maximum length of a message
#define OVERHEAD 70
#define SENDPIN 8
#define TAKEPIN 9
