#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <malloc.h>
#include <sys/wait.h>

#define CLONES 4
#define STACKSIZE 5000
#define ITERATIONS 2000000000

//TODO: Not calculating anything. Need to fix that
double PI = 0;
double pi_array[CLONES] = {0};
int n = 0;

int PIcalculation(void * args);

int main(){
    char *stackhead[CLONES];
    char *stack[CLONES];
    int arrayArg[CLONES];
    int status[CLONES];
    int i;
    long long startTime;
    long long endTime;
    float elapsed_time;
    struct timeval ts;

    gettimeofday(&ts, NULL);
    startTime = ts.tv_sec * 1000000 + ts.tv_usec; //Start

    for(i=0; i<CLONES ; i++){
        stack[i] = (char *)malloc(STACKSIZE);
    }
	
	for(i=0; i<CLONES ; i++){
        stackhead[i] = stack[i] + STACKSIZE -1;
    }

    if(!stack){
        printf("Error allocating stack\n");
        exit(1);
    }

    for(i = 0; i<CLONES; i++){
        arrayArg[i] = i;
        clone(PIcalculation, stackhead[i], SIGCHLD | CLONE_VM , &arrayArg[i]);
    }
    
	
    for(i = 0; i<CLONES; i++)
        wait(&status[i]);
	

    gettimeofday(&ts, NULL);
    endTime = ts.tv_sec * 1000000 + ts.tv_usec; //End

    PI = 4*(pi_array[0] + pi_array[1] + pi_array[2] + pi_array[3]);
    elapsed_time = (float) (endTime - startTime)/1000000.0;
    printf("pi is equal to %1.10f\n",PI);
    printf("Elapsed time = %2.2f seconds\n",elapsed_time);

    for(i = 0; i<CLONES; i++)
        free(stack[i]);

    return 0;
}

int PIcalculation(void *args){
    int nchild = *((int *)args);
    int iterationStart = (nchild)*(ITERATIONS/CLONES);
    int iterationEnd = (nchild+1)*(ITERATIONS/CLONES);
    double sum = 0.0;
    signed short toggle = 0;
    double pi;
    int i;

    for(i = iterationStart; i < iterationEnd; i++){
        toggle = !toggle;
        sum = (double)((toggle*2)-1)/((2*i) + 1);
        pi = pi + sum;
    }
	
    pi_array[nchild] = pi;

	return 0;
}