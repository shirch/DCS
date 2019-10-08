
#include "TFC.h"

extern Max_Degrees, Zero_Degrees, opMode, Ten_degrees_Shift;

void  ServosConfig_Max();
void ServosConfig_Min();
void decrementTenPrecentShift();
void incrementTenPrecentShift();
void incrementTenPrecentShift_Pit();
void decrementTenPrecentShift_Pit();

int Min_Motor0 = 950, Min_Motor1 = 1200; 
extern counter , servo1State, servo2State;

//for bump
void  ServosConfig_Max(){//both motors are in the front of the car looking strait

	TPM1_C1V = Zero_Degrees; // 0 degrees
	TPM0_C4V = Max_Degrees; // 180 degrees	
	servo1State = servo2State = 0; //indicator for both motors are strait
}

//for scanning
void ServosConfig_Min(){//both motors are at 90 degrees of the car looking aside
	
	TPM1_C1V  = Min_Motor1; // 90 degrees
	TPM0_C4V  = Min_Motor0; // 90 degrees
	servo1State = servo2State = 1;//indicator for both motors are looking aside
}

void servoTurnLeft(){
	TPM1_C1V = Zero_Degrees; // 0 degrees
	TPM0_C4V  = Min_Motor0; // 90 degrees
}
//for exit
void servoTurnRight(){
	TPM1_C1V  = Min_Motor1; // 90 degrees
	TPM0_C4V = Max_Degrees; // 180 degrees
}
//FOR TESTS//
/*
void decrementTenPrecentShift(){//moving towards the front of the car from Min to Max
	
	RGB_LED_OFF;
	
		if (TPM1_C1V >= Zero_Degrees)
			 TPM1_C1V -= Ten_degrees_Shift; // increment of 10 degrees
		else
			 TPM1_C1V = Zero_Degrees;
		
		if(TPM0_C4V <= Max_Degrees)
			 TPM0_C4V += Ten_degrees_Shift; // increment of 10 degrees
		else
			 TPM0_C4V = Max_Degrees;
		int i;
		for (i = 0; i <5000; i++);//dealy
		GREEN_LED_ON;
}
		

void incrementTenPrecentShift(){//moving towards the sides of the car from Max to Min
	
	RGB_LED_OFF;
	
		if (TPM1_C1V <= Max_Degrees)
			TPM1_C1V += Ten_degrees_Shift;
		else
			 TPM1_C1V = Max_Degrees;
	
		if(TPM0_C4V >= Zero_Degrees)
			 TPM0_C4V -= Ten_degrees_Shift; // decrement of 10 degrees
		else
			 TPM0_C4V = Zero_Degrees;
		int i;
		for (i = 0; i <0xffff; i++);//dealy
		RED_LED_ON;
		
}

void incrementTenPrecentShift_Pit(){//this function enables us to scan the area 
	

	opMode = 2;
	counter = 1;
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; //enable the PIT module			
}

void decrementTenPrecentShift_Pit(){//this function enables us to scan the area 
	
	opMode = 3;
	counter = 1;
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; //enable the PIT module
			
}

*/
