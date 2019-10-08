/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include <stdio.h>
#include <stdlib.h>
# include "TFC.h"

/*the arena array for location and maping. 
0-not scanned/not mapped/not stepped
1-stepped
2-empty
3-obstacle
*/
int arr[45][60] = {0}; 
int currRow = 22;// start location
int currCol = 0;//start location
//int direction = 0;// 0-straight, 1- right, 2- left,3- reverse
int driveMode = 3;//scanning-0,bump-1,pass-3, return-4, search wall-6, search exit-7, finish-8
int RotationDirection=0;//0-staright, 1-right, 2-left;
extern RisingEdge1, RisingEdge2;

int main(void){
	
	ClockSetup();
	InitGPIO();
	RGB_LED_OFF;
	InitTPM(0);
	InitTPM(1);
	InitTPM(2);
	InitTPM(3);
	ServosConfig_Max();
	dealy();
	InitPIT();//starts the increment / decrement by scanning, time among interrupts is 4ms;	
	maxSpeed();
	dealy();
	InitPIT1();// - ultra sonic
	
	initColorSensor();

	
	while(1)
	{
	}
	return 0;
}



