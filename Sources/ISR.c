//------------------------------------------------------------------------------------------------//
// 									 ISR-interrupts file
//------------------------------------------------------------------------------------------------//
#include "TFC.h"

/*//___________________________________Strat of VARIABLES DEFINITIONS___________________________________________\\*/
//for mapping
/*
extern arr;
extern currCol, currRow, direction;
*/
/*//_______________General - For All Clocks_____________\\*/
int MUDULO_REGISTER = 0x493E; //18750 100% duty cycles 

/*//_______________FOR DC MOTORS_____________\\*/
float DistanceToTarget = 150.0;//for entrance door
float DistanceObstacels = 300.0;
int IntDistanceToTarget = 150;//for entrance door
extern int Rotation;
extern int driveMode;
#define  fractionRight	32 //changes by the current floor
#define  fractionLeft	16 //changes by the current floor

/*//_______________For UART Menu_____________\\*/
static int pressProtector = 0; //when entering the menu of the UART

/*//_______________For ADC + IR Sensors_____________\\*/
static double analogVoltage;
static double MaxVoltage = 4095;
static int countSamp6 = 0;
static int countSamp7 = 0;
static int distance6[32] = { 0 };
static int distance7[32] = { 0 };
static int countIR = 0;
static int currSamp[5]; //5 samples to do average in real time
static int ch = 31; //channel se6b/se7b
static int IRstate = 0; //0-sampling for graph, 1- real time sampling
static int sampCount6 = 0;
static int sampCount7 = 0;
static int monePit = 0;
#define  Xcm	10

/*//_______________For Input Cpature_____________\\*/
char a, b;
int currentVelocity1 = 1, currentVelocity2 = 1;

/*//_______________For Servo Motors_____________\\*/
int opMode = 1;
int Zero_Degrees = 480; //{Modulo_register / (25 milSec/0.568 milSec)} -> actually little more than 0 degrees but we want to prevent collision 
int Max_Degrees = 1600; //{Modulo_register / (25 milSec/2.2 milSec)} -> actually little less than 180 degrees but we want to prevent collision
int Ten_degrees_Shift = 55;  //(Max_Degrees-Zero_Degrees)/18 
static int Servo_Counter = 0; //counts how many time we already entered the pit's interrupt'

/*//_______________For ULTRA SONIC __________________\\*/
int tmpo;
extern int Sonic1_Distance, Sonic2_Distance, RisingEdge1, RisingEdge2;
int counter = 1; //for one at the time 
/*//_______________For mapping __________________\\*/
extern currRow,currCol;
/*//___________________________________END OF VARIABLES DEFINITIONS___________________________________________\\*/


//-----------------------------------------------------------------
// PIT - ISR = Interrupt Service Routine - TMP2_C1V = PWM
//-----------------------------------------------------------------
void PIT_IRQHandler(void) {

	if (PIT_TFLG0 == 1) { //flag up- 31bit - pit0 - servo

		if (Rotation == 0)
		{
			Inpute_Capture_ENC1();
			Inpute_Capture_ENC2();

			if (currentVelocity1 < currentVelocity2)//currentVelocity2 = TPM2_C1V, currentVelocity1 =  TPM1_C0V
			{
				TPM2_C1V -= (currentVelocity2 - currentVelocity1) * fractionRight; 
				TPM1_C0V += (currentVelocity2 - currentVelocity1) * fractionRight; 
			}
			else if (currentVelocity1 > currentVelocity2)//currentVelocity2 = TPM2_C1V, currentVelocity1 =  TPM1_C0V
			{
				TPM2_C1V += (currentVelocity1 - currentVelocity2) * fractionLeft; 
				TPM1_C0V -= (currentVelocity1 - currentVelocity2) * fractionLeft; 
			}
			
			float AvgVelocity = 0;

			if(currentVelocity1 < 60 && currentVelocity2 < 60)
			{
				if(currentVelocity1 > 0 && currentVelocity2 > 0)
				AvgVelocity = ((currentVelocity1+currentVelocity2)*0.004);
			}

			else if (currentVelocity1 < 60)
			{
				if(currentVelocity1 > 0 )
				AvgVelocity = currentVelocity1*0.008;
			}
			else if (currentVelocity2 < 60)
			{
				if(currentVelocity2 > 0)
				AvgVelocity = currentVelocity2*0.008;
			}
			else
			AvgVelocity = 0.336;

			if(driveMode == 3) //pass 150 cm
			{
				DistanceToTarget = DistanceToTarget - AvgVelocity;
				IntDistanceToTarget = ((int) DistanceToTarget);

				if (DistanceToTarget <= 0) //passed 1.5 meter
				{
					ServosConfig_Min();
					driveMode=0; //scanning mode

				}
			}

			if(driveMode == 0) //scanning mode
			{
				DistanceObstacels = DistanceObstacels - AvgVelocity;
				IntDistanceToTarget = ((int) DistanceObstacels);

				if (DistanceObstacels < 0) //passed 3 meter - obstacle area
				{
					driveMode=6; //Look for exit - door
					ServosConfig_Max();
				}
			}
		}

		PIT_TFLG0 = PIT_TFLG_TIF_MASK; //Turn off the Pit 0 Irq flag 
	}

	if(PIT_TFLG1== 1) { //flag up- 31bit - pit1 - ultraSonic //SAMPLE BOTH STRAIT

		PIT_TFLG1 = PIT_TFLG_TIF_MASK;//Turn off the Pit 1 Irq flag

		RisingEdge1 = 1;
		RisingEdge2 = 1;
		initUltraSonic();

		PIT_MCR |= PIT_MCR_MDIS_MASK;// disable the pit

	}
}

void dealy() {
	int i = 0;
	for (i = 0; i < 0x1FFFFF; i++);
}

/*
//MAPPING FUNCTIONS//
//if the distance diffrence greater than 10cm return 1-true else return 0-false
int didWePassSquare(int start,int done)
{
	if((done-start)>10)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void locationMap()
{
	switch (driveMode){
	case 0://scanning
		if(didWePassSquare(oldDistanceObstacels,DistanceObstacels))
		{
			currCol++;
			arr[currRow][currCol] = 1;
		}
		break;
	case 1://bump
		if(didWePassSquare(oldDistanceObstacels,DistanceObstacels))
		{
			currRow++;
			arr[currRow][currCol] = 1;
		}
		break;
	case 6://wall
		if(didWePassSquare(oldDistanceObstacels,DistanceObstacels))
		{
			currCol++;
			arr[currRow][currCol] = 1;
		}
		break;
	case 7://exit
		if(didWePassSquare(oldDistanceObstacels,DistanceObstacels))
		{
			currRow++;
			arr[currRow][currCol] = 1;
		}
		break;
	case 3://pass
		if(didWePassSquare(oldDistanceToTarget,DistanceToTarget))
		{
			currCol++;
			arr[currRow][currCol] = 1;
		}
		break;
	default :

	}
}
*/
