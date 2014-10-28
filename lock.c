#include "3140_concur.h"

extern process_t *current_process;

// Reset the lock
void l_init(lock_t *plockr)
{
	if(plockr) // check if the lock is valid
	  plockr->owned=0;  // set the owner of the lock to NULL
}

// Sets the owner for the lock and sets the lock that the process wants
// If the lock is free, take the lock
// If the lock is taken, waits for it to become free and then takes it
void l_lock(lock_t *plockr)
{						// begin l_lock
	if(plockr){
	  __disable_interrupt(); // disable interrupt while modifiying the lock 
	  
	  if(plockr->owned==0) // check if the lock is owned by another process
	  {// not owned
	  	plockr->owned=current_process; // set the current process as the owner
	  }
	  else
	  {// owned
	  		current_process->owner = plockr; // the current process wants the lock	
	   		process_blocked(); // waiting for the lock to become available				
			plockr->owned = current_process; // the current process becomes the owner of this lock
    	}
	
	 __enable_interrupt();
	}
}						//end l_lock

// Releases the lock and clears the process waiting for the lock
void l_unlock(lock_t *plockr)
{						//begin l_unlock
    if(plockr)
    {
      __disable_interrupt();
      	
	  current_process->owner = 0;	// the current process doesn't want the lock
	  plockr->owned=0; // free the lock
	
	  __enable_interrupt();
    }

}						//end l_unlock
