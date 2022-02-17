
#ifndef _ASSIGNMENT_BODY_
#define _ASSIGNMENT_BODY_

#define PIN_BUTTON 0
#define PIN_A_IR_SEND 5
#define PIN_A_IR_RECV 6
#define PIN_B_IR_SEND 8
#define PIN_B_IR_RECV 9

typedef struct shared_variable {
	int bProgramExit; // Once it is set to 1, the program will be terminated.
	int button;
} SharedVariable;


void init_shared_variable(SharedVariable* sv);
void init_sensors(SharedVariable* sv);
void body_button(SharedVariable* sv); // Button
void body_a_ir_send(SharedVariable* sv);
void body_a_ir_recv(SharedVariable* sv);
void body_b_ir_send(SharedVariable* sv);
void body_b_ir_recv(SharedVariable* sv);

#endif
