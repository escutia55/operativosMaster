#include "scheduler.h"
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		_enqueue(&ready,callingthread);
	}

	if(event == TIMER){
		_enqueue(&ready, currthread); //Send current thread to the queue
		changethread=1;
	}
	
	if(event==BLOCKTHREAD)
	{
		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);

		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
	}
	
		
	if(changethread)
	{
		old=currthread;
		next=_dequeue(&ready); //Get newer thread out of the queue
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

}
