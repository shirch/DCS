#include "TFC.h"
#define STEP 0x0735 // MUDULO_REGISTER/10
static int opMode = 1;
static int Current_Direction = 0;
int Rotation = 0; // will be changed to 1 only if we are rotating
extern currentVelocity1, currentVelocity2, MUDULO_REGISTER,
		ClicksCounter_Motor1, ClicksCounter_Motor2;
extern MaxStrait, NeededNumOfClicks; // how many clicks do we need for 
static int distanceOneClick_cm = 4.62; //2*pi*R / 408 *100 (*100 because it is to small for the cpu)
extern servo2State, servo1State, Sonic1_Distance, Sonic2_Distance; //sonic sensors strait or not
int StopsCounter;//for location iin the arena

/*//_______________Functions in the file_____________\\*/

int getDirection();
void Motor_Dir_and_Speed(int Direction, int Speed);
void maxSpeed();
void stopMoving();
void RotateRight();
void RotateLeft();
void Setspeed();

/*//_______________END of Functions in the file_____________\\*/

void Motor_Dir_and_Speed(int Direction, int Speed) {

	switch (Direction) {
	case 0:	//forward MOTOR1 & MOTOR2
		GPIOC_PDOR = 0;
		GPIOC_PDOR |= PORT_LOC(10) + PORT_LOC(6);		//0x0440
		break;
	case 1:	//backward MOTOR1 & MOTOR2
		GPIOC_PDOR = 0;
		GPIOC_PDOR |= PORT_LOC(7) + PORT_LOC(5);		//0xA0
		break;
	case 2: //turn right 	
		TPM2_C1V = MUDULO_REGISTER/2; //100% duty cycles / prescaler (= Speed)
		GPIOC_PDOR |= PORT_LOC(6);		//0x40
		break;
	case 3: //turn left 
		TPM1_C0V = MUDULO_REGISTER/2;//100% duty cycles / prescaler (= Speed)
		GPIOC_PDOR |= PORT_LOC(10);		//0x400
		break;
	default:	//stops
		GPIOC_PDOR = 0;
		TPM2_C1V = TPM1_C0V = 0;
		break;
	}

	if (Direction < 2)
	{
		TPM2_C1V = MUDULO_REGISTER / Speed; //100% duty cycles / prescaler (= Speed)
		TPM1_C0V = MUDULO_REGISTER / Speed;//100% duty cycles / prescaler (= Speed)
	}

}

int getDirection() {
	return Current_Direction;
}

void maxSpeed(int DriveDistance) {

	Rotation = 0;
	Motor_Dir_and_Speed(0, 2); //goes forward - second number is an arbitrary because we don't use speed variable in the function 
	
}


void stopMoving() {

	Rotation = 0;
	GPIOC_PDOR = 0; //stop moving (can't change the TPM0_CnV so fast)

}

void RotateLeft() {

	stopMoving(); //we turn from stand still
	Rotation = 1;
	Motor_Dir_and_Speed(2, 1); //turn right 100% speed
}

void RotateRight() {
	
	stopMoving(); //we turn from stand still
	Rotation = 1;
	Motor_Dir_and_Speed(3, 1); //turn right 100% speed
}
void reverse() {
	
	Rotation = 0;
	Motor_Dir_and_Speed(1, 2); //turn right 100% speed
}
