#include <stdio.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "sender.h"

#define thread_def(NAME) \
int bExit_##NAME; \
void* thread_##NAME(void* param) { \
	SharedVariable* pV = (SharedVariable*) param; \
	workload_##NAME(); \
	body_##NAME(pV); \
	bExit_##NAME = 1; \
	return NULL; }

void signal_handler(int signum) {
	finish_workload();
	release_buffer(fileno(stdout));
	printf("\nProgram exits by SIGINT.\n");
	exit(0);
}

int main(int argc, char* argv[]) {
	SharedVariable v;

	int runningTimeInSec = 10;
	if (argc == 2) {
		runningTimeInSec = atoi(argv[1]);
	}

	if (wiringPiSetup() == -1) {
		printf("Failed to setup wiringPi.");
		return 1; 
	}

	// Initialize for the interfaces provided
	signal(SIGINT, signal_handler);
	init_deferred_buffer(256*1024*1024); // 128MB
	init_userspace_governor();
	init_workload();

	// Initializers that you need to implement
	//init_shared_variable(&v);
	init_sensors(&v);
	//learn_workloads(&v);

	set_by_max_freq(); // reset to the max freq

	while (v.bProgramExit != 1) {
		// 1. Prepare tasks
		idleTime = prepare_tasks(aliveTasks, &v);
		// printDBG("idleTime: %lld\n", idleTime);
		if (idleTime < 0)
			break;

		// 2. Select a process: You need to implement.
		sel = select_task(&v, aliveTasks, idleTime); 

		// If idle time is not zero, directly goto the next round
		if (idleTime > 0)
			continue;

		// 3. Run the selected task
		execute_task(sel);
	}

	finish_workload();
	release_buffer(fileno(stdout));
	printf("Scheduler program finished.\n");

	return 0;
}
