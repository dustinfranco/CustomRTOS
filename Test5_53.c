#include "3140_concur.h"

lock_t l;

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
		P1OUT^=0x02;
	}



void p1(void){

	while(1)
	{
	P1OUT^= 0x01;
	delay();
	}
	}


int main(void){
	WDTCTL = WDTPW + WDTHOLD;
	P1DIR = 0x03;
	P1OUT = 0x00;

	l_init (&l);
	
	if (process_rt_create (rtp1,10,1000,10,1000) < 0) {
	 	return -1;
	}
	if (process_create (p1,10) < 0) {
	 	return -1;
	}
	
	process_start();
	P1OUT= 0x02;
	while(1){};
	return 0;	
}
