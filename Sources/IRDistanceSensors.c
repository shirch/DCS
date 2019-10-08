
#include "TFC.h"

#define	numOfSamples	16
#define	contNumOfSamples	150
#define sampAvg		5

static int continousDistance6[contNumOfSamples];
static int continousDistance7[contNumOfSamples];
/////////////////////////////////////////////////////////Added Functions\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void configIR(){
	adc_init();
	
	PORTD_PCR6 = PORT_PCR_MUX(0);// PTD6 TO ADC0_SE7b
	PORTD_PCR5 = PORT_PCR_MUX(0);// PTD5 TO ADC0_SE6b
	ADC0_SC1A &= ~ADC_SC1_ADCH(31);
}
void DistanceMeasure (int Distance[] , uint16_t samples , uint8_t Xcm, int channel) //samples - size of Distance
{
	
	Distance[samples] = ADC0_RA;
	/*
	if (samples == numOfSamples-1)//last time enter
	{
		LinearCountinous(Distance, Xcm,channel);
	}
	*/
	
}

void LinearCountinous (int Distance[], uint8_t Xcm,int channel)
{
	int index;
	int plotIndex=0;
	for(index=0;index<numOfSamples;index++)
	{
		int j;
		for(j=0;j<Xcm;j++)
		{
			if(channel==6)
			{
				continousDistance6[plotIndex]=Distance[index]+(Distance[index+1]-Distance[index])*j/Xcm;
			}
			else
			{
				continousDistance7[plotIndex]=Distance[index]+(Distance[index+1]-Distance[index])*j/Xcm;
			}
			plotIndex++;
		}
	
	}
	
}

//searches the  required Voltage value and returns the index of the correct distance. 
int searchVoltage6()
{
	int currentVoltage = ADC0_RA;
	int i;
	for(i=0;i<contNumOfSamples;i++)
	{
		if(abs(currentVoltage-continousDistance6[i])<0.1)
		{
			return i;
		}
		
	}
	return -1;//not a valid voltage
}

//searches the  required Voltage value and returns the index of the correct distance. 
int searchVoltage7()
{
	int currentVoltage = adc_read(7);
	int i;
	for(i=0;i<contNumOfSamples;i++)
	{
		if(abs(currentVoltage-continousDistance7[i])<0.1)
		{
			return i;
		}
		
	}
	return -1;//not a valid voltage
}

