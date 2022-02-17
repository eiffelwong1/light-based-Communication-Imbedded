// Important! DO NOT MODIFY this file.
// You will not submit this file.
// This file is provided for your understanding of the program procedure.

// Skeleton code of CSE237A, Sensor interaction
// For more details, please see the instructions in the class website


#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "sensors.h"

#define thread_decl(NAME) \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	body_##NAME(pV); \
	return NULL; }

thread_decl(button)
thread_decl(a_ir_send)
thread_decl(a_ir_recv)
thread_decl(b_ir_send)
thread_decl(b_ir_recv)

#define thread_create(NAME) pthread_create(&t_##NAME, NULL, thread_##NAME, &v);
#define thread_join(NAME) pthread_join(t_##NAME, NULL);

int main(int argc, char* argv[]) {
	// Initialize
	SharedVariable v;

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.");
		return 1; 
	}

	init_shared_variable(&v);
	init_sensors(&v);

	pthread_t t_button,
		  t_a_ir_send,
		  t_a_ir_recv,
		  t_b_ir_send,
		  t_b_ir_recv;

	while (v.bProgramExit != 1) {
		// Create sensing threads
		thread_create(button);
		thread_create(a_ir_send);
		thread_create(a_ir_recv);
		thread_create(b_ir_send);
		thread_create(b_ir_recv);

		// Wait until all threads finish
		thread_join(button);
		thread_join(a_ir_send);
		thread_join(a_ir_recv);
		thread_join(b_ir_send);
		thread_join(b_ir_recv);

		delay(1);
	}

	printf("Program finished.\n");

	return 0;
}
