#include "scheduler.h"
#include <stdio.h>
#define MAX_PRIORITIES 5

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready[6]; //Holds five levels of priorities
QUEUE waitinginevent[MAXTHREAD];

int smaller(int *array, int count){ //Count refers to the number of threads
    int i;
    for(i = 0; i <count-1; i++){ //Only check for the smallest priority in the current threads
        if(array[i] < array[i+1]){
            array[i+1] = array[i];
        }
    }
    return array[i];
}

void scheduler(int arguments)
{
	int old;
	static int next = 0;
	int changethread=0;
	int waitingthread=0;
	static int smallestPriority;
	static int priotiy_array[4] = {0}; //Holds the priority of each process
	static int threads_count = 1;
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;
	int i;

	if(event==NEWTHREAD)
	{
		threads[callingthread].status=READY;
		_enqueue(&ready[0],callingthread); //New thread to priority 0
		threads_count +=1; 

	}

	if(event == TIMER){
		priotiy_array[next] += 1; //Increase process's priority
		for(i = 0; i<= 3; i++){
			if(priotiy_array[i] >= MAX_PRIORITIES){
				priotiy_array[i] = 5; //Prevent a priority overflow 
			}
		}

			_enqueue(&ready[priotiy_array[next]], currthread); //Send current thread to queue +1 priority 
			changethread = 1;
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
		_enqueue(&ready[smallestPriority],callingthread); //Send thread with smalles priority
	}

		
	if(changethread)
	{
		old=currthread;
		smallestPriority = smaller(priotiy_array, threads_count);
		next=_dequeue(&ready[smallestPriority]);		
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}
}
