
#include "TFC.h"
extern RotationDirection;
extern driveMode;
int SawBlack=0;
void initColorSensor() {
	//TODO color sensors do not respond
	PORTE_PCR2 = PORT_PCR_MUX(1); //sensor 1
	//PORTE_PCR2 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0xB);
	GPIOE_PDDR &= ~PORT_LOC(2);
	
	PORTE_PCR3 = PORT_PCR_MUX(1); //sensor 2
	//PORTE_PCR3 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0xB);	
	GPIOE_PDDR &= ~PORT_LOC(3);
	
	PORTE_PCR4 = PORT_PCR_MUX(1); //sensor 3
	//PORTE_PCR4 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0xB);
	GPIOE_PDDR &= ~PORT_LOC(4);
	
	PORTE_PCR5 = PORT_PCR_MUX(1); //sensor 4
	//PORTE_PCR5 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0xB);
	GPIOE_PDDR &= ~PORT_LOC(5);
	
	PORTA_PCR16 = PORT_PCR_MUX(1); //sensor 5
	//PORTA_PCR16 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0xB);
	GPIOA_PDDR &= ~PORT_LOC(16);
}

void black()
{
		switch (driveMode){
		case 0://scanning
			straight();
			break;
		case 1://bump - no black strip

			break;
		case 2://door- no black strip

			break;
		case 3://pass - no black strip

			break;
		case 4://return
			searchBlack();
			break;


		}
		
}
void straight()
{
	if(RotationDirection=0)
	{
		if(!(GPIOE_PDIR & PORT_LOC(4)))//middle
		{

			if(GPIOE_PDIR & PORT_LOC(2))//left edge
			{
				TPM2_C1V-=4;
				TPM1_C0V+=4;
			}	
			else if(GPIOE_PDIR & PORT_LOC(3))//left
			{
				TPM2_C1V-=2;
				TPM1_C0V+=2;
			}


			if(GPIOA_PDIR & PORT_LOC(16))//right edge
			{
				TPM2_C1V+=4;
				TPM1_C0V-=4;
			}
			else if(GPIOE_PDIR & PORT_LOC(5))//right
			{
				TPM2_C1V+=2;
				TPM1_C0V-=2;
			}
		}
	}
}


void searchBlack()
{
	if(GPIOE_PDIR & 0x0000003C)
	{
		SawBlack= 1;
	}
	else
	{
		SawBlack= 0;
	}
	
	//////////for testing\\\\\\\
	
	/*
	int blackLine=0;
	if(GPIOE_PDIR & PORT_LOC(2))//left edge
	{
		RGB_LED_OFF;
		GREEN_LED_ON;
	}
	if(GPIOE_PDIR & PORT_LOC(3))//left
	{
		RGB_LED_OFF;
		BLUE_LED_ON;
	}
	if(GPIOE_PDIR & PORT_LOC(4))//middle
	{
		RGB_LED_OFF;
		RED_LED_ON;
	}
	if(GPIOE_PDIR & PORT_LOC(5))//right
	{
		RGB_LED_OFF;
		GREEN_LED_ON;
		BLUE_LED_ON;
	}
	if(GPIOA_PDIR & PORT_LOC(16))//right edge
	{
		RGB_LED_OFF;
		BLUE_LED_ON;
		RED_LED_ON;
	}
	
	blackLine |=(GPIOE_PDIR & PORT_LOC(2))/4;
	
	blackLine |=(GPIOE_PDIR & PORT_LOC(3))/4;
	
	blackLine |=(GPIOE_PDIR & PORT_LOC(4))/4;
	
	blackLine |=(GPIOE_PDIR & PORT_LOC(5))/4;
	
	blackLine |=(GPIOA_PDIR & PORT_LOC(16))/4096;

	return blackLine;
*/

}
