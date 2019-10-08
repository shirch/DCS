#include "TFC.h"

/*//_________________________Functions and Interrupts in the file_________________\\*/

void Inpute_Capture_ENC1();
void Inpute_Capture_ENC2();
void FTM0_IRQHandler();
void scanObstacle();
void TurningBackToEncoders();
void printEnc2();
void printEnc1();
void printingfunction();
void bumpObstacle();
void driveModeControl();

/*//_________________________END of Functions and Interrupts in the file_____________\\*/

/*//____________________________VARIABLES DEFINITIONS_______________________\\*/
extern SawBlack;
//encoders + DC motors variables && externs
extern int currentVelocity1, currentVelocity2, MUDULO_REGISTER, Rotation;
static int timeElapsed1 = 0, timeElapsed2 = 0; // (secondCounter1 - firstCounter1)/MUDULO_REGISTER; delta counter * period Time 
static int Motor1_Old = 0, Motor1_New = 0, Motor2_Old = 0, Motor2_New = 0; //counters which t (1/2) is updated
int TOF1 = 0, TOF2 = 0, tmp1 = 0, tmp2 = 0;
static int distanceOneClick = 31950; //34650; // //cm (2*Pi*R/408 clicks)*TimeOneUpTPM (18750*40)

//ultra sonic variables && externs
int sonicTOF1 = 0, sonicTOF2 = 0, Sonic1 = 0, Sonic2 = 0, RisingEdge1 = 1,
		Sonic1_Distance = 0, Sonic2_Distance = 0, RisingEdge2 = 1, Sonic1_tag =
				0, Sonic2_tag = 0;
char a, b, c;
int tmpo, degel1 = 0, degel2 = 0; //degel = finished sampling
int servo1State = 0, servo2State = 0; //0-forward , 1=90 degrees to the relevant side
extern int counter;
extern int driveMode;
extern RotationDirection;
static int moneObstacle1 = 0, moneObstacle2 = 0, moneExit = 0, moneAfterReturn = 0;
static int twoObstacles = 0, counterObstacles4=0; //1-both sides obstacles

extern int IR1_Distance, IR2_Distance; 

/*//_________________________END OF VARIABLES DEFINITIONS________________________\\*/

//-----------------------------------------------------------------
// PTM0 - ISR = Interrupt Service Routine
//-----------------------------------------------------------------
void FTM0_IRQHandler() {
	// getting the capture and counter values

	if (TPM0_STATUS & 0x08) //CH3 Interrupt
			{
		TPM0_STATUS |= 0x08; //Resets the interrupt flag of the channel;
		TPM0_C3SC |= TPM_CnSC_CHF_MASK; //resets interrupt flag at the channel

		if (TPM0_MOD == 0xC350 && degel1 == 0) //ultra sonic
				{

			if (RisingEdge1 == 1) //from low to high
					{
				RisingEdge1 = 0;
				sonicTOF1 = 0;
				Sonic1 = TPM0_C3V ; //the value of the timer when we sent it

					}
					else //from high to low
					{
						RisingEdge1 = 1;
						Sonic1_tag = TPM0_C3V;
						if (sonicTOF1 == 1)
						{
							Sonic1_tag += 0xC350;
						}
						else if (sonicTOF1 == 2)
						{
							Sonic1_tag += 2*0xC350;
						}

						Sonic1 = (Sonic1_tag - Sonic1); //the delta counter after returning
						Sonic1_Distance = Sonic1*0.0227;// - 8;// the distance measure by the formula
						degel1 = 1;

					}
				}

				else if ( TPM0_MOD != 0xC350 ) //encoders
				{
					Motor1_Old = tmp1; //now the new is not updated
					tmp1 = Motor1_New = TPM0_C3V;//sets the TMP current value

					if (TOF1 == 1)
					Motor1_New += MUDULO_REGISTER;
					else if (TOF1 == 2)
					Motor1_New += 2*MUDULO_REGISTER;

					TOF1 = 0;//Resets the counter for ENC1

					PIT_MCR &= ~PIT_MCR_MDIS_MASK;// enable the PIT module	
				}

			}

			else if (TPM0_STATUS & 0x04) //CH2 Interrupt - motor2 TPM2_C1V  
			{
				TPM0_STATUS |= 0x04; //Resets the interrupt flag of the channel;
				TPM0_C2SC |= TPM_CnSC_CHF_MASK;//resets interrupt flag at the channel

				if (TPM0_MOD == 0xC350 && degel2 == 0)//ultra sonic
				{

					if (RisingEdge2 == 1) //from low to high
					{
						RisingEdge2 = 0;
						sonicTOF2 = 0;
						Sonic2 = TPM0_C2V; //the value of the timer when we sent it

					}
					else //from high to low
					{
						RisingEdge2 = 1;
						Sonic2_tag = TPM0_C2V;
						if (sonicTOF2 == 1)
						{
							Sonic2_tag += 0xC350;
						}
						else if (sonicTOF2 == 2)
						{
							Sonic2_tag += 2*0xC350;
						}

						Sonic2 = (Sonic2_tag - Sonic2); //the delta counter after returning
						Sonic2_Distance = Sonic2*0.0227;// - 8;// the distance measure by the formula

						degel2 = 1;

					}
				}
				else if (TPM0_MOD != 0xC350) //encoders
				{
					Motor2_Old = tmp2; //now the new is not updated
					tmp2 = Motor2_New = TPM0_C2V;//sets the TMP current value

					if (TOF2 == 1)
					Motor2_New += MUDULO_REGISTER;
					else if (TOF2 == 2)
					Motor2_New += 2*MUDULO_REGISTER;

					TOF2 = 0;//Resets the counter for ENC2
					PIT_MCR &= ~PIT_MCR_MDIS_MASK;// enable the PIT module	
				}
			}

			else if (TPM0_STATUS & 0x0100 || TPM0_SC & 0x80) //TOF
			{
				TPM0_STATUS |= 0x0100; //resets TOF flag at STATUS 
				TPM0_SC |= 0x80;//resets TOF flag at SC
				if(TPM0_MOD != 0xC350)
				{
					TOF1++;
					TOF2++;
				}
				else if (TPM0_MOD == 0xC350)
				{
					sonicTOF2++;
					sonicTOF1++;
				}
			}

			if ((TPM0_MOD == 0xC350 && degel1 + degel2 == 2) || (TPM0_MOD == 0xC350 && sonicTOF1 + sonicTOF2 > 2 ))
			{
				if (TPM0_STATUS & 0x08)
				TPM0_STATUS |= 0x08; //Resets the interrupt flag of the channel;
				if (TPM0_STATUS & 0x04)
				TPM0_STATUS |= 0x04;//Resets the interrupt flag of the channel;
				if(TPM0_STATUS & 0x0100 || TPM0_SC & 0x80)
				{
					TPM0_STATUS |= 0x0100; //resets TOF flag at STATUS 
					TPM0_SC |= 0x80;//resets TOF flag at SC
				}
				if(sonicTOF2 > 2 || Sonic2_Distance > 120)
				Sonic2_Distance = 900;
				if(sonicTOF1 > 2 || Sonic1_Distance > 120)
				Sonic1_Distance = 900;

				printingfunction();

				switch (driveMode) {
					case 0: //scanning
					black();
					maxSpeed();
					scanObstacle();
					break;
					case 1://bump
					bumpObstacle();
					break;
					case 4://return black line
					reverse();
					returnScanning();
					break;
					case 6:// search wall
					maxSpeed();
					searchWall();
					break;
					case 7:
					maxSpeed();// search exit
					searchExit();
					break;
					case 8://done
					maxSpeed();
					break;
					default :
					maxSpeed();
				}

			}
		}

void Inpute_Capture_ENC1() {

	timeElapsed1 = Motor1_New - Motor1_Old;

	currentVelocity1 = distanceOneClick / timeElapsed1; //cm/se

}

void Inpute_Capture_ENC2() {

	timeElapsed2 = Motor2_New - Motor2_Old;

	currentVelocity2 = distanceOneClick / timeElapsed2; //cm/se

}

void TurningBackToEncoders() {

	PORTE_PCR31 = PORT_PCR_MUX(3);
	TPM0_SC = 0; // to ensure that the counter is not running
	TPM0_SC |= TPM_SC_PS(5) + TPM_SC_TOIE_MASK;//Prescaler = 32, up-mode, counter-disable
	TPM0_C2SC = 0;
	TPM0_C2SC |= TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK;
	TPM0_C3SC = 0;
	TPM0_C3SC |= TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK;
	TPM0_MOD = MUDULO_REGISTER;
	PORTC_PCR3 = PORT_PCR_MUX(4);//– TPM0_CH2 
	PORTC_PCR4 = PORT_PCR_MUX(4);//– TPM0_CH3
	PORTE_PCR31 = PORT_PCR_MUX(3);//– TPM0_CH4
	TPM0_C1SC = 0;
	PORTD_PCR1 = PORT_PCR_MUX(0);// default
	PORTD_PCR2 = PORT_PCR_MUX(0);// default
	PORTD_PCR3 = PORT_PCR_MUX(0);// default
	RisingEdge1 = 1;
	RisingEdge2 = 1;
	TPM0_SC |= TPM_SC_CMOD(1);// start the clock


}

void printingfunction() {

	UARTprintf(UART0_BASE_PTR, "current distance motor1:  ");

	if (Sonic1_Distance > 99) {
		tmpo = Sonic1_Distance / 100;
		c = tmpo + 48;
		UARTprintf_CHAR(UART0_BASE_PTR, c);
		Sonic1_Distance = Sonic1_Distance - tmpo * 64 - tmpo * 32 - tmpo * 4; //easier only shifts
	}

	tmpo = Sonic1_Distance / 10;
	a = tmpo + 48;
	tmpo = Sonic1_Distance - tmpo * 8 - tmpo * 2; //easier only shifts
	b = tmpo + 48;

	UARTprintf_CHAR(UART0_BASE_PTR, a);
	UARTprintf_CHAR(UART0_BASE_PTR, b);
	UARTprintf(UART0_BASE_PTR, "   [cm/sec]\r\n");

	//print motor2 velocity//
	UARTprintf(UART0_BASE_PTR, "\r\n current distance motor2:  ");
	int tmp = Sonic2_Distance;
	if (tmp > 99) {
		tmpo = tmp / 100;
		c = tmpo + 48;
		UARTprintf_CHAR(UART0_BASE_PTR, c);
		tmp = tmp - tmpo * 64 - tmpo * 32 - tmpo * 4; //easier only shifts
	}

	tmpo = tmp / 10;
	a = tmpo + 48;
	tmpo = tmp - tmpo * 8 - tmpo * 2; //easier only shifts
	b = tmpo + 48;

	UARTprintf_CHAR(UART0_BASE_PTR, a);
	UARTprintf_CHAR(UART0_BASE_PTR, b);
	UARTprintf(UART0_BASE_PTR, "  [cm/sec]\r\n");
	printEnc1();
	printEnc2();
}


void bumpObstacle() {
	if (Sonic1_Distance < 15 && Sonic2_Distance < 15 ){//|| (IR1_Distance == 15 && IR2_Distance == 15)) {
		stopMoving();
		driveMode = 4;
		TurningBackToEncoders();
	} else {
		TurningBackToEncoders();
	}
}


void scanObstacle() {
	if (Sonic2_Distance < 100 && counterObstacles4<4) {//if right sensor saw an obstacle
	switch (twoObstacles){
		case 0://no obstacles
			if (moneObstacle2 == 2) {
				ServosConfig_Max();
				dealy();
				dealy();
				moneObstacle2 = 0;
				RotateLeft();
				dealyRotationLeft(0);
				driveMode = 1; //bump
				RotationDirection = 1; //right
				stopMoving();
				dealy();
				dealy();
				dealy();
				maxSpeed();
				twoObstacles=1;//right flag on
				TurningBackToEncoders();

			} else if (moneObstacle2 < 2) {
				moneObstacle2++;
			}
		break;
		case 3://left
			twoObstacles=2;
		break;
		case 4://go to right after handling left
			if (moneObstacle2 == 2) {
				ServosConfig_Max();
				dealy();
				dealy();
				moneObstacle2 = 0;
				RotateLeft();
				dealyRotationLeft(0);
				driveMode = 1; //bump
				RotationDirection = 1; //right
				stopMoving();
				dealy();
				dealy();
				dealy();
				maxSpeed();
				TurningBackToEncoders();

			} else if (moneObstacle2 < 2) {
				moneObstacle2++;
			}
			twoObstacles=1;//right
			break;
		case 7://done with obstacles continue drive for some samples
			if(moneAfterReturn==10)
			{
				moneAfterReturn=0;
				twoObstacles=0;	
		
				if (moneObstacle2 == 2) {
					ServosConfig_Max();
					dealy();
					dealy();
					moneObstacle2 = 0;
					RotateLeft();
					dealyRotationLeft(0);
					driveMode = 1; //bump
					RotationDirection = 1; //right
					stopMoving();
					dealy();
					dealy();
					dealy();
					maxSpeed();
					TurningBackToEncoders();

				} else if (moneObstacle2 < 2) {
					moneObstacle2++;
				}
			}
			else if(moneAfterReturn<10)
			{
				moneAfterReturn++;
			}
						
		break;
	}
}
	if (Sonic1_Distance < 100  && counterObstacles4<4) {
	switch (twoObstacles){
		case 0://no more obstacles
			if (moneObstacle1 == 2) {
				ServosConfig_Max();
				dealy();
				dealy();
				moneObstacle1 = 0;
				RotateRight();
				dealyRotationRight(0);
				driveMode = 1; //bump
				RotationDirection = 2; //left
				stopMoving();
				dealy();
				dealy();
				dealy();
				maxSpeed();
				twoObstacles=3;//left
				TurningBackToEncoders();
			} else if (moneObstacle1 < 2) {
				moneObstacle1++;
			}
			
		break;
		case 1://right
			twoObstacles=2;
		break;
		case 5://go to left after handling right
			if (moneObstacle1 == 2) {
				ServosConfig_Max();
				dealy();
				dealy();
				moneObstacle1 = 0;
				RotateRight();
				dealyRotationRight(0);
				driveMode = 1; //bump
				RotationDirection = 2; //left
				stopMoving();
				dealy();
				dealy();
				dealy();
				maxSpeed();
				TurningBackToEncoders();

			} else if (moneObstacle1 < 2) {
				moneObstacle1++;
			}
			twoObstacles=3;//left
			break;
		case 7://done with obstacles continue drive for some samples

			if(moneAfterReturn==10)
			{
				moneAfterReturn=0;
				twoObstacles=0;	
		
				if (moneObstacle1 == 2) {
					ServosConfig_Max();
					dealy();
					dealy();
					moneObstacle1 = 0;
					RotateRight();
					dealyRotationRight(0);
					driveMode = 1; //bump
					RotationDirection = 2; //left
					stopMoving();
					dealy();
					dealy();
					dealy();
					maxSpeed();
					TurningBackToEncoders();

				} else if (moneObstacle1 < 2) {
					moneObstacle1++;
				}
			}
			else if(moneAfterReturn<10)
			{
				moneAfterReturn++;
			}

		break;
	}
	}
}




void returnScanning() //driveMode=4
{

	black();
	if (SawBlack == 1) {
		if (RotationDirection == 1) //right
		{
			ServosConfig_Min();
			RotateRight();
			dealyRotationRight(0);
			stopMoving();
			dealy();
			dealy();
			RotationDirection =0;
			if(twoObstacles==1)
			{
				twoObstacles = 7;//go to handle other side
			}
			else if (twoObstacles==2)
			{
				twoObstacles=5;//go to left
			}
			SawBlack=0;
		}
		if (RotationDirection == 2) //left
			{
			ServosConfig_Min();
			RotateLeft();
			dealyRotationLeft(0);
			stopMoving();
			dealy();
			dealy();
			RotationDirection =0;
			if(twoObstacles==3)
			{
				twoObstacles = 7;
			}
			else if (twoObstacles==2)
			{
				twoObstacles=4;//go to right
			}
			SawBlack=0;
		}
		driveMode = 0; //scanning
		counterObstacles4++;//done with another obstacle
	}

	
}

void searchWall() {
	if (Sonic1_Distance < 30 && Sonic2_Distance < 30) {
		stopMoving();
		servoTurnRight(); //1 servo look at wall, 2 servo straight
		RotateRight();
		dealyRotationRight(0);
		stopMoving();
		driveMode = 7;
		RGB_LED_OFF;
	}
		else
	 {
	 	TurningBackToEncoders();
	 }
}

void searchExit() {
	if (Sonic1_Distance > 60) //noticed the exit-hole
			{
		if (moneExit == 4) {
			moneExit = 0;
			stopMoving();
			ServosConfig_Max(); //1 servo look at wall, 2 servo straight
			RotateLeft();
			dealyRotationLeft(0);
			stopMoving();
			TurningBackToEncoders();
			RGB_LED_ON;
			driveMode = 8;
		} else {
			moneExit++;
		}
	}
		else
	 {
	 	TurningBackToEncoders();
	 }
}

void printEnc1() {
	Inpute_Capture_ENC1();

	//print motor1 velocity//
	int tmp = currentVelocity1 / 10;
	a = tmp + 48;
	tmp = currentVelocity1 - tmp * 8 - tmp * 2; //easier only shifts
	b = tmp + 48;

	UARTprintf(UART0_BASE_PTR, "current velocity motor1:  ");
	UARTprintf_CHAR(UART0_BASE_PTR, a);
	UARTprintf_CHAR(UART0_BASE_PTR, b);
	UARTprintf(UART0_BASE_PTR, "   [cm/sec]\r\n");
}

void printEnc2() {

	Inpute_Capture_ENC2();

	//print motor2 velocity//
	int tmp = currentVelocity2 / 10;
	a = tmp + 48;
	tmp = currentVelocity2 - tmp * 8 - tmp * 2; //easier only shifts
	b = tmp + 48;

	UARTprintf(UART0_BASE_PTR, "\r\n current velocity motor2:  ");
	UARTprintf_CHAR(UART0_BASE_PTR, a);
	UARTprintf_CHAR(UART0_BASE_PTR, b);
	UARTprintf(UART0_BASE_PTR, "  [cm/sec]\r\n");

	char* direction = (int) getDirection() + 48;
	UARTprintf(UART0_BASE_PTR, "\r\n	current direction motor1: \r\n");
	UARTprintf(UART0_BASE_PTR, direction);
}
