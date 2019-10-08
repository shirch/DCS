#include "TFC.h"
#include "mcg.h"


void InitGPIO();
void InitTPM(char x);
void ClockSetup();
void InitPit();
void InitPit0Op1();
void InitPit0Op2();
void dealyRotationLeft();
void dealyRotationRight();


extern Max_Degrees, Zero_Degrees, MUDULO_REGISTER, TOF1, TOF2;

void InitGPIO()
{
	//enable Clocks to all ports - page 206, enable clock to Ports
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
//------------------------------------------------------------------------------------------	
	//GPIO Configuration - Direction , Current Sample (Input) & Angle - Output
//------------------------------------------------------------------------------------------

	PORTC_PCR5 =  PORT_PCR_MUX(1); // assign PTC5 as GPIO == M_IN_P
	PORTC_PCR6 =  PORT_PCR_MUX(1); // assign PTC6 as GPIO == M_IN_N 
	PORTC_PCR7 =  PORT_PCR_MUX(1); // assign PTC7 as GPIO == M_IN_P
	PORTC_PCR10 = PORT_PCR_MUX(1); // assign PTC10 as GPIO == M_IN_N 
	
	GPIOC_PDDR = 0x04E0; //set as outputs - M_IN_N, M_IN_P (ptc5,ptc6,ptc7,ptc10)
	GPIOC_PDOR = 0x00; //set outputs to 0

	//Setup as PWM - PTE23,PTE20
	PORTE_PCR23 = PORT_PCR_MUX(3); //assign PTE23 as GPIO connected to TMP2_CH1
	PORTE_PCR20 = PORT_PCR_MUX(3); //assign PTE20 as GPIO connected to TMP1_CH0
	
	//Angle setup - PTE21,PTE31
	PORTE_PCR21 = PORT_PCR_MUX(3); //assign PTE21 as GPIO connected to TMP1_CH1
	PORTE_PCR31 = PORT_PCR_MUX(3); //assign PTE31 as GPIO connected to TMP0_CH4
	
	//ADC featured pins - Current Samples - INPUT (by default)
//	PORTE_PCR29 = PORT_PCR_MUX(0); //Default operation mode - ADC_SE4b
//	PORTE_PCR30 = PORT_PCR_MUX(0); //Default operation mode - ADC_SE23
	
//------------------------------------------------------------------------------------------	
	//GPIO Configuration - encoder  - Input + interrupt of Input Capture enable
//------------------------------------------------------------------------------------------	
	
	PORTC_PCR3 = PORT_PCR_MUX(4); // assign PTC3 as GPIO connected to TMP0_CH2
	PORTC_PCR4 = PORT_PCR_MUX(4); // assign PTC4 as GPIO connected to TMP0_CH3
	
//------------------------------------------------------------------------------------------	
	//GPIO Configuration - LEDs - Output
//------------------------------------------------------------------------------------------	
	
	PORTD_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;  //Blue
	GPIOD_PDDR |= BLUE_LED_LOC; //Setup as output pin	
	PORTB_PCR18 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Red  
	PORTB_PCR19 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK; //Green
	GPIOB_PDDR |= RED_LED_LOC + GREEN_LED_LOC; //Setup as output pins
	

//------------------------------------------------------------------------------------------
	//GPIO Configuration - Pushbutton - Input
//------------------------------------------------------------------------------------------	

	PORTD_PCR7 = PORT_PCR_MUX(1); // assign PTD7 as GPIO
	GPIOD_PDDR &= ~PORT_LOC(7);  // PTD7 is Input
	PORTD_PCR7 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0x0a);
	
/*	PORTD_PCR6 = PORT_PCR_MUX(1); // assign PTD7 as GPIO
	GPIOD_PDDR &= ~PORT_LOC(6);  // PTD7 is Input
	PORTD_PCR6 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0x0a);*/
	
	PORTD_PCR5 = PORT_PCR_MUX(1); // assign PTD7 as GPIO
	GPIOD_PDDR &= ~PORT_LOC(5);  // PTD7 is Input
	PORTD_PCR5 |= PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_IRQC(0x0a);
	
	enable_irq(INT_PORTD-16); // Enable Interrupts 
	set_irq_priority (INT_PORTD-16,0);  // Interrupt priority = 0 = max
	
	
//------------------------------------------------------------------------------------------
}

/*
//-----------------------------------------------------------------
// DipSwitch data reading
//-----------------------------------------------------------------
uint8_t TFC_GetDIP_Switch()
{
	uint8_t DIP_Val=0;
	
	DIP_Val = (GPIOC_PDIR>>4) & 0xF;

	return DIP_Val;
}
*/

//-----------------------------------------------------------------
// TPMx - Initialization
//-----------------------------------------------------------------
void InitTPM(char x){  // x={0,1,2}
	switch(x){
	case 0://Encoders
		
		//General for all channels of the module TPM0
		TPM0_SC = 0; // to ensure that the counter is not running
		TPM0_MOD = MUDULO_REGISTER; // PWM frequency of 40Hz = 24MHz/(64x9375)
		TPM0_CONF = 192;
			
		//Channel 1
	
		TPM0_C1SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK;
		TPM0_C1V = 0xA;//12 micro sec
		
		//Channel 2 - Encoder 1 - input capture mode
		TPM0_C2SC |= TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK; //Rising Edge Only + Enable channel interrupt 
		TPM0_C2V = 0x00;//0% Duty cycles 
			
		//Channel 3 -Encoder 2 - input capture mode
		TPM0_C3SC |= TPM_CnSC_ELSA_MASK + TPM_CnSC_CHIE_MASK; //Rising Edge Only + Enable channel interrupt
		TPM0_C3V = 0x00;//0% Duty cycles*/
			
		//Channel 4
		TPM0_C4SC = TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK;
		TPM0_C4V = Max_Degrees;//180 degrees
				
		break;
		
	case 1: //motor 2
		
		//General for all channels of the module TPM1
		TPM1_SC = 0; // to ensure that the counter is not running
		TPM1_SC |= TPM_SC_PS(5)+TPM_SC_TOIE_MASK; //Prescaler =32, up-mode, counter-disable
		TPM1_MOD = MUDULO_REGISTER; // PWM frequency of 40Hz = 24MHz/(64x9375)
		TPM1_CONF = 0;
				
		//Channel 0
		TPM1_C0SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		TPM1_C0V = 0; 
				
		//Channel 1
		TPM1_C1SC = TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK + TPM_CnSC_CHIE_MASK;
		TPM1_C1V = Zero_Degrees; //Zero degrees
				
				
		TPM1_SC |= TPM_SC_CMOD(1); //Start the TPM1 counter;

	case 2: //motor 1
		
		TPM2_SC = 0; // to ensure that the counter is not running
		TPM2_SC |= TPM_SC_PS(5)+TPM_SC_TOIE_MASK; //Prescaler =32, up-mode, counter-disable
		TPM2_MOD = MUDULO_REGISTER; // PWM frequency of 40Hz = 24MHz/(64x18750)
		TPM2_C1SC |= TPM_CnSC_MSB_MASK + TPM_CnSC_ELSB_MASK;
		TPM2_C1V = 0; 
		TPM2_CONF = 0;
				
				
		TPM2_SC |= TPM_SC_CMOD(1); //Start the TPM2 counter
		break;
		
	case 3: //ultra-sonic sensors

		//General for all channels of the module TPM0
		TPM0_SC = 0; // to ensure that the counter is not running
		TPM0_SC |= TPM_SC_PS(5)+TPM_SC_TOIE_MASK; //Prescaler =32, up-mode, counter-disable
		TPM0_MOD = 0xAFBE; // PWM frequency of 10Hz = 24MHz/(64x18750x2)
		TPM0_CONF = 0;
						

		
		TOF1 = TOF2 = 0; //reset overflow counters
		
		enable_irq(INT_TPM0-16); //Enable TPM0 interrupt
	    set_irq_priority(INT_TPM0-16,2);  // Interrupt priority = 0 = max
		TPM0_SC |= TPM_SC_CMOD(1); //Start the TPM0 counter
		break;
	
	}
}
//-----------------------------------------------------------------
// TPMx - Clock Setup
//-----------------------------------------------------------------
void ClockSetup(){
	    
	    pll_init(8000000, LOW_POWER, CRYSTAL,4,24,MCGOUT); //Core Clock is now at 48MHz using the 8MHZ Crystal
		
	    //Clock Setup for the TPM requires a couple steps.
	    //1st,  set the clock mux
	    //See Page 124 of f the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;// We Want MCGPLLCLK/2=24MHz (See Page 196 of the KL25 Sub-Family Reference Manual
	    SIM_SOPT2 &= ~(SIM_SOPT2_TPMSRC_MASK);
	    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); //We want the MCGPLLCLK/2 (See Page 196 of the KL25 Sub-Family Reference Manual
		//Enable the Clock to the TPM0 and PIT Modules
		//See Page 207 of f the KL25 Sub-Family Reference Manual
		SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK +  SIM_SCGC6_TPM1_MASK + SIM_SCGC6_TPM2_MASK;
	    // TPM_clock = 24MHz , PIT_clock = 48MHz
	    
}

//-----------------------------------------------------------------
// PIT - Initialisation
//-----------------------------------------------------------------
void InitPIT(){
	
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	// Timer 0
	PIT_LDVAL0 = 0x00017700;//0x0002EE00; // setup timer 8ms period
	PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,1);  // Interrupt priority = 0 = max
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	
	
}

//-----------------------------------------------------------------
// PIT - Initialisation
//-----------------------------------------------------------------
void InitPIT1(){
	
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	// Timer 1
	PIT_LDVAL1 = 0x800000;//0x0002EE00; // setup timer 0 for maximum counting period
	//PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,1);  // Interrupt priority = 0 = max
	PIT_TCTRL1 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; // enable the PIT module	
	
	
}


/*void InitPit0Op1(){
	
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	// Timer 0
	PIT_LDVAL0 = 0x01680000;//0x0002EE00; // setup timer 8ms period
	PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,0);  // Interrupt priority = 0 = max
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; // enable the PIT module	
}

void InitPit0Op2(){
	
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK; //Enable the Clock to the PIT Modules
	// Timer 0
	PIT_LDVAL0 = 0x01527777;//0x0002EE00; // setup timer 8ms period
	PIT_MCR |= PIT_MCR_FRZ_MASK; // stop the pit when in debug mode
	enable_irq(INT_PIT-16); //  //Enable PIT IRQ on the NVIC
	set_irq_priority(INT_PIT-16,0);  // Interrupt priority = 0 = max
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK; //enable PIT0 and its interrupt
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; // enable the PIT module	
}*/

void EnableInputCapture(){
			
	//interrupt enable
	enable_irq(INT_TPM0-16); //Enable TPM0 interrupt
    set_irq_priority(INT_TPM0-16,3);  // Interrupt priority = 0 = max
    TPM0_SC |= TPM_SC_PS(5) + TPM_SC_TOIE_MASK; //Prescaler =32, up-mode, counter-disable
	TPM0_SC |= TPM_SC_CMOD(1); //Start the TPM0 counter
	
}

void dealyRotationLeft(){//calculated by radius of the wheels and the raduis of the distance among them
	
int i = 0;
for (i = 0; i< 0x0350000; i++);
stopMoving();

}

void 	dealyRotationRight(){//calculated by radius of the wheels and the raduis of the distance among them
	
int i = 0;
for (i = 0; i < 0x0450000; i++);
stopMoving();

}
