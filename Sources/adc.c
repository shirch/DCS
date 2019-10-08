
#include "TFC.h" 

int IR1_Distance, IR2_Distance, ConvretedDistance; 

void InitADC0(){
	SIM_SCGC6|= SIM_SCGC6_ADC0_MASK;
	
	ADC0_CFG1= 0;
	ADC0_CFG1 |= (ADC_CFG1_MODE(1)|
				ADC_CFG1_ADICLK(0)|
				ADC_CFG1_ADIV(1));
	
	ADC0_CV1 = 0x0000;
	ADC0_CV2 = 0xFFFF;
	
	ADC0_SC2 = 0;
	ADC0_SC2 |= ADC_SC2_ACREN_MASK + ADC_SC2_ACFE_MASK + ADC_SC2_ACFGT_MASK + ADC_SC2_REFSEL(1);
	ADC0_SC1A = 0;
	ADC0_SC1A |= ADC_SC1_ADCH(31) + ADC_SC1_AIEN_MASK;
	
	ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK;
	
	enable_irq(INT_ADC0-16);
	set_irq_priority(INT_ADC0-16,0);
}



 
/*
VoltDistanceCal[0] = 0.0;
VoltDistanceCal[5] = 0.0;
VoltDistanceCal[10] = 0.0;
VoltDistanceCal[15] = 2.58;
VoltDistanceCal[20] = 2.32;
*/



void SampleRight(){
	
	ADC0_SC1A &= ~ADC_SC1_ADCH(31);
	ADC0_SC1A |= ADC_SC1_ADCH(7);
	ConvretedDistance = (ADC0_RA * 3.3)/4095;//Distance by Voltage
	if (ConvretedDistance >= 2.40)
		IR1_Distance = 15;
	

}
void SampleLeft(){
	
	ADC0_SC1A &= ~ADC_SC1_ADCH(31);
	ADC0_SC1A |= ADC_SC1_ADCH(6);
	ConvretedDistance = (ADC0_RA * 3.3)/4095;//Distance by Voltage
	if (ConvretedDistance >= 2.40)
		IR2_Distance = 15;
	
}
