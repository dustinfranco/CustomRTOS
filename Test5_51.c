#include "3140_concur.h"

//this simulates a processor running fast and a processor running slow
//is simulated in 5_52
//the amonut of delays is meant to simulate how fast the processor is running
//this processor finishes it's work in a handful of delay,
//in 5_52 it will take many more delays
int x=1;
void delay (void){
	int i= 0;
	while (i < 8000) {
		__no_operation();
		__no_operation();
		__no_operation();
		i++;
	}
}

void rtp1(void)
	{		
		delay();
		delay();
		x=0;
	}



void rtp2(void){
if(x==1)
{
P1OUT = 0x01;
}
else
{
P1OUT=0x02;
}
}
void p1(void)
{while(1){}}

int main(void){
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR = 0x03;
	P1OUT = 0x00;

	
	if (process_rt_create(rtp1,20,50,10,10000) < 0) 
	{
	 	return -1;
	}
	if (process_rt_create(rtp2,20,2000,10,1000) < 0) 
	{
	 	return -1;
	}


	if (process_create (p1,10) < 0) {
	 	return -1;
	}
	
	process_start();
	//P1OUT= 0x02;
	while(1){};
	return 0;	
}

