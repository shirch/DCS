

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define TPMMOD 0x927C

int ultraStartVals[2];
int ultraOverflowVals[2];
float ultraDistance[2];

int newUltraMeasure[2];
int UltraMesuresCounter[2];

float UltraAverage[2];

int ultraRealPulses[2];

void initUltraSonic();

int overflowCounter;


#endif /* ULTRASONIC_H_ */
