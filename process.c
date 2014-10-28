#include "3140_concur.h"
#include <stdlib.h>

/*
Things to Make

(X)Real Time List Struct
(X)add RT process creation
(X)RTL add helper (making progress, should probably do process creation first)
(X)RTL remove helper
(X)Edit Process select
()Set up timer b clocks


Extra Credit
For the extra credit we have to implement a helper function that moves the process further down the list.

*/


//**************************//
//	  List related things   //
//**************************//


/*Asignment 3 and 4*/
typedef struct orderlist	
{				
	process_t *val;			
	struct orderlist *norder; 	
} order;		
	


/*Assignment 5*/
typedef struct {   unsigned int sec;   unsigned int msec; } realtime_t;

//timer ints
int current_time_msec = 0, current_time_sec = 0;

//rt create declaration
process_rt_create(void (*f)(void), int n, realtime_t *start, realtime_t *work, realtime_t *deadline);

//Real Time List
typedef struct rtorderlist	
{				
	process_t *val;			
	struct orderlist *norder; 	
	int etim=0;
	int deadline;
} rtorder;			

rtorder rtexecuting;


//**************************//
//	  Initializing Things   //
//**************************//

/*Assignment 3 or 4*/
order *forder=0; 			  //first order in the queue (forder)
process_t *current_process=0; //current process being executed

/*Assignment 5*/

//this is hte head of our rt queue
//it will ONLY change in three places:
//adding the first order in the list
//if a process is added during process creation that has an earlier deadline
//it is finished executing during process select (ONLY TIME IT HAS TO CHANGE IN REAL TIME)
rtorder *rtforder=0;





//*********************************//
//     Realtime Queue Functions    //
//*********************************//
//helper functions for the realtime queue, NOT realtime process creation, that's further down.

//In process select, this inserts the process to the real time queue
//it will sort as it inserts, so we will not have to swap them later 
//looks similar to our non real time, but it's sorted and changes less often since there is a priority of processes


void rtplusorder(rtorder *rtlorder)
{
	int curtime=time();
	rtorder *tmp;
	tmp=rtforder;
	rtorder *tmp=rtforder;
	rtorder *tmptwo;
	//list is empty
	if(rtforder==0)
	{
		rtforder = rtlorder
		rtlorder->norder = 0;
	}
	
	//the process we are adding has the lowest possible deadline and thus is the new head of our list
	else if((rtlorder->etim)<(rtforder->etim))
	{
		rtlorder->norder=tmp;
		rtforder=rtlorder;	
	}
	else
	{	
		while((tmp->norder)&&((tmp->etim)>lorder->etim)))
		{
			tmptwo=tmp;
			tmp=tmp->norder;
		}
		if(tmp->norder==0)
		{
			rtlorder->norder=0;
			tmp->norder=rtlorder;
		}
		else
		{
			rtlorder->norder=tmp;
			tmptwo->norder=rtlorder;
		}
		
	}
}

//rtminus

rtminusorder(rtorder *rtlorder)
{
	rtorder *tmp;
	*tmp=rtforder;
	if(rtforder->norder==0)
	{
		current_process=0;
		rtforder=0;
	}
	else if (rtforder==rtlorder)
	{
		rtforder=rtlorder->norder;
	}
	else 
	{
		while(tmp->norder!=rtlorder)
		{
			tmp->norder=rtlorder->norder;
		}
	}


}

//*********************************//
// Non Realtime Queue Functions    //
//*********************************//
//attaching an order to the end of the order list
void plusorder(order *lorder) 
{					//PLUS ORDER BEGIN
	
    order *tmp;//temporary variable, only vsisble in the scope of plusorder, type order (list node)
    	
    if (forder == 0) // this is hte first process added to the queue
	{     
        forder = lorder;     //the first order is this process
        lorder->norder = 0;  //no second process is declared, so the next item of the list is nothing
    }  
    else //not the first process added to the queue
	{     
		tmp = forder;     //store the front of the queue in tmp
        while (tmp->norder) //check to see if there's a pointer to the next thing in the list
		{   				//if there isn't we've reached the end of the list
            /* while there are more elements in the list */       
            tmp = tmp->norder;     //clever way to iterate through the list until the end, given on BB
        }     
            /* now tmp is the last element in the list */     
		tmp->norder = lorder;     //the end of the list should point to the order we are adding
		lorder->norder = 0; 	  //since we are putting it at the end of the list, the next thing in the list is nothing
    }  		
}					//PLUS ORDER END

//Remove the process from the beginning of the list
void minusforder(void)
{					//MINUS ORDER BEGIN
	//check to see if there's anything in the front of the list
           if (forder == 0) 
		   {      
               current_process=0;//if there isn't set current_process to 0
           }  
           else  //There is something at the front of the list
		   {     
               forder = forder->norder;  //set the new front of the list to the thing that is second in line   
               current_process=forder->val;  //set the current process to the new front of the list's process
           }  
	
}					//MINUS ORDER END

//Find the next process in the queue that either has the lock or doesn't need the lock to run
void find_owner(void)
{
	order *tempo; //temporary pointer to an order
	
	// swtich to the next process if the current process is waiting for a lock that it doesn't own
	while(current_process->owner!=0 && current_process->owner->owned!=0 && current_process->owner->owned!=current_process)
	{
			tempo = forder;				//fill the temporary order struct with the first order
			minusforder(); 				//get rid of the process in first order
			plusorder(tempo); 			//add what WAS the first order to the last slot
	}
}

//**************************//
//	  Process Select        //
//**************************//
// process select called during a context switch (invoked in 3140.ASM)
// always call on find_owner if the queue is not empty
// release the lock when the current process is done

unsigned int process_select (unsigned int cursp){
		rtorder *tmp;
		
		//ASSIGNMENT 5
		curtim=time();
		if(rtforder)
		{
			if(cursp==0&&current_process!=0) 
			{
				minusrtorder(rtexecuting);
			}
			if(rtforder)
			{
				tmp=rtforder;
				if((rtforder->stim)>curtim)
				{
					current_process=rtforder->val;
					rtexecuting=rtforder;
					return current_process->sp;
				}
				else
				{
					while((tmp->norder)&&((tmp->stim)<curtim))
					{
						tmp=tmp->norder;
					}
					if(!(tmp->norder==0&&tmp->stim<curtim))
					{
						current_process=tmp->val;
						rtexecuting=tmp;
						return current_process->sp;
					}
				}
			}
		}	
		//ASSIGNMENT 3 and 4
	
		//**********//
		//six cases //
		//**********//
		
		//CASE 1
		
		//nothing running (or has ever), but there's something at the front of the list
		//EG process terminated and there's another thing to execute
		if((cursp==0)&&(current_process==0)&&(forder!=0))
		{
			current_process=forder->val;//make the current process the process_t (val) pointed to by the first order in the list
			find_owner();
			return current_process->sp; //return the stack pointer of the current process
		}
		
		//CASE 2
		
		//nothing running, nothing in the queue, nothing ever
		//EG (initializing)
		if((cursp==0)&&(current_process==0)&&(forder==0))
		{
			return 0;//ain't nothin' can be done but waitin'.
			// But also if there were code in main other than infinite while, it would execute that code while waiting for the processes be added to the queue
		}
		
		//CASE 3
		
		//current process has finished, and there is a process in the queue waiting to execute
		//don't add it to the end of queue, it's done, replace it forever
		//EG process has finished during it's latest time on the processor, now just let it die :(
		if((cursp==0)&&(current_process!=0)&&(forder->norder!=0))
		{							
	        current_process->owner = 0; //release the lock when the current process finishes
	
			forder = forder->norder; //the new first order is what it is pointing to in it's norder property
			current_process = forder->val; //the new current process is the process stored in what was formerly the second order in the queue
            find_owner();      
			return current_process->sp; //return the SP stored in the queue_t structure (val) of the first order (AKA current_process)
		}
		
		//CASE 4
		//current process is done running, but there's nothing in the queue left
		//EG, the last thread left running in the program is has finished	
		if((cursp==0)&&(current_process!=0)&&(forder->norder==0))
		{	
			current_process->owner = 0; // release the lock 
							  //shut
			current_process=0;//it
			forder=0;		  //all
			return 0;		  //down
			//(set everything to zero, on the next process switch it will probably enter into case 3
			//so long as another process is not added
		}
		
		//CASE 5
		//current process is not done running at time of process select
		//but luckily there's nothing in the queue, so it can continue doing it's thing :)
		//EG last process in the program is running, while every other process has finished
		if ((cursp!=0)&&(forder->norder==0))
		{
			current_process->sp = cursp;//the new current process should stay the same as when it entered
			return cursp;     //no context switch because there's no reason to switch context
		}
		
		//CASE 6
		//Current process is not finished, but there's something in the queue that wants time
		//in the processor, so switch to that and put the current process at the end of the queue
		// EG, after the first time in the processor, it's unlikely a process is done, so give it to the second one
		//and wait in line to take another ride on the CPU
		
		else //also this catches anything that wasn't in cases 1-5 (which SHOULD be nothing)
		{
			current_process->sp = cursp;//set the current process stack pointer to cursp
			temp = forder;				//fill the temporary order struct with the first order
			minusforder(); 				//get rid of the process in first order
			plusorder(temp); 			//add what WAS the first order to the last slot, because it's not done yet
            find_owner();
			return current_process->sp;     //return the pointer to the relevant process (stored within val within the new first order)
		}
}




void process_start (void)
{

//TIMER A STUFF
    TACTL = TASSEL_2+ID_3+MC_1; //use the SMCLK input,  divide down the clock with 3, count up
	TACCR0 = 65000;				//set the clock timer to 65000 ( TACCR0 is 16 bit counter - max value is 65535)
	//The two folling lines are for testing, comment out the line above this and uncomment one of the two lines below to shorten the period between timer A0 interrupts
	//TACCR0 = 15000;  //FAST SWITCHING
	//TACCR0 = 500; //VERY FAST SWITCHING
	
	TACCTL0 = CCIE;				//set timer A0 interrupt
	
	
//TIMER B STUFF
	
	
	
	
	
	_disable_interrupt();		//self explanatory
	process_begin();			//begin the processes! Huzzah!
}

//creates a new concurrent process starting at function f with 
//initial stack size n
//returns 0 if successful, -1 otherwise

/*process create assignment 3 or 4 */
int process_create (void (*f)(void), int n)
{
	unsigned int new_sp;	//sp of the new process 		

	order *newproc = (order *)malloc(sizeof(order)); //create a new node for the list
	newproc->val = (process_t *)malloc(sizeof(process_t)); //create the new process
	
	//initialize the new process's lock
	newproc->val->owner = 0;
	
	//create a new sp for the new process
	_disable_interrupt(); //disable interupts before call on process_init
	new_sp = process_init(f,n);
	_enable_interrupt();
	
	//if process_init fails to allocate the memory
	if(new_sp==0)
	{
		return -1;
	}
	
	//if memory allocation succeeds 
	newproc->val->sp=new_sp; //setup the sp for the new process
	plusorder(newproc); //add process to the list
	return 0;
}

//**************************//
//	  Other Assignment 5    //
//**************************//

//Process Create Assignment 5

process_rt_create(void (*f)(void), int n, realtime_t *start, realtime_t *work, realtime_t *deadline)
{
	unsigned int new_sp;	//sp of the new process 		

	if(work>(start+deadline))
	{
		return -1;
	}
	
	rtorder *newproc = (rtorder *)malloc(sizeof(rtorder)); //create a new node for the list
	newproc->val = (process_t *)malloc(sizeof(process_t)); //create the new process
	
	//initializing the rtorder
	newproc-> stim=start;
	newproc-> etim=start+deadline;
	newproc-> norder=0;
	
	_disable_interrupt(); //disable interupts before call on process_init
	new_sp = process_init(f,n);
	_enable_interrupt();
	
	if(new_sp==0)
	{
		return -1;
	}
	
	newproc->val->sp=new_sp; //setup the sp for the new process
	rtplusorder(newproc); //add process to the list
	return 0;
	
	//if process_init fails to allocate the memory

	
	
	
	
}

//ASG5
int time()
{
	return current_time_msec + 1000*current_time_sec;
}


//ASG5
#pragma vector= TIMERB0_VECTOR
__interrupt void timer_interrupt()
{
	if (++current_time_msec >= 1000)
	{
		current_time_sec++;
		current_time_msec = 0;
	}
}