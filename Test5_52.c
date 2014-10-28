#include "3140_concur.h"

//this simulates a processor running slow
//p1 won't update x in time, so the LED will be red once P2 runs
//the amonut of delays is meant to simulate how fast the processor is running
//this processor is "slow" so it takes a long time (many delays) to get to the updating x
//if it were faster it would not have this issue (start time doesn't depend on processor speed)
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
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
		delay();
		delay();		
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

