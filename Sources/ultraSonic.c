
#include "TFC.h"

extern degel1,degel2;

void initUltraSonic(){//we are ready to sample
	
		degel1 = degel2 = 0;//we havn't sampled yet
		PORTC_PCR3 = PORT_PCR_MUX(0);  //– TPM0_CH2  //disconnect enc
		PORTC_PCR4 = PORT_PCR_MUX(0);  //– TPM0_CH3 //disconnect enc
		PORTE_PCR31 = PORT_PCR_MUX(0); //turns off configuration to one of the servo motors.		
		TPM0_SC = 0;
		PORTD_PCR1 = PORT_PCR_MUX(4);  // TPM0_CH1 //connect
		PORTD_PCR2 = PORT_PCR_MUX(4);  // TPM0_CH2 - ultra
		PORTD_PCR3 = PORT_PCR_MUX(4);  // TPM0_CH3 - ultra
		TPM0_SC |= TPM_SC_PS(5) + TPM_SC_TOIE_MASK; // clock divide by 32 + over flow enable 
		TPM0_MOD = 0xC350; // freq = 15Hz (50000)
		TPM0_C3SC = 0;
		TPM0_C3SC |= TPM_CnSC_ELSB_MASK + TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK; //Capture on rising and falling
		TPM0_C2SC = 0; //starts with channel 3. we can't work in parallel
		TPM0_C2SC |= TPM_CnSC_ELSB_MASK + TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK; //Capture on rising and falling
		TPM0_C1SC = 0;
		TPM0_C1SC |= TPM_CnSC_ELSB_MASK + TPM_CnSC_MSB_MASK; // PWM
		TPM0_C1V = 0xFA; // 18 micro-sec 
		TPM0_SC |= TPM_SC_CMOD(1); // start clock
}

