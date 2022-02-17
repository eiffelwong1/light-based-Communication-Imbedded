#include "sensors.h"
#include <stdio.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>

void init_shared_variable(SharedVariable* sv){
	sv->button = 0;
}

void init_sensors(SharedVariable* sv){
	if(wiringPiSetup() == -1){
		printf("setup wiringPi failed!");
	}

	pinMode(PIN_BUTTON, INPUT);

	pinMode(PIN_A_IR_SEND, OUTPUT);
	pinMode(PIN_A_IR_RECV, INPUT);

	pinMode(PIN_B_IR_SEND, OUTPUT);
	pinMode(PIN_B_IR_RECV, INPUT);
}

void body_button(SharedVariable* sv) {
	sv->button = !digitalRead( PIN_BUTTON );
	printf("button: %d ", sv->button);
}

void body_a_ir_send(SharedVariable* sv){
	digitalWrite( PIN_A_IR_SEND, sv->button);
}

void body_a_ir_recv(SharedVariable* sv){
	return;
}

void body_b_ir_send(SharedVariable* sv){
	return;
}

void body_b_ir_recv(SharedVariable* sv){
	printf("B_IR_RECV: %d %d \n", PIN_B_IR_RECV , digitalRead( PIN_B_IR_RECV ) );
}

void print_state_console(SharedVariable* sv){

}
