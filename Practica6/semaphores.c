#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include "semaphores.h"
#define atomic_xchg(A,B) __asm__ __volatile__( \
" lock xchg %1,%0 ;\n" \
: "=ir" (A) \
: "m" (B), "ir" (A) \
);
#define CICLOS 10

void proceso(int i);

char *pais[3] = {"Peru","Bolivia","Colombia"};
int semaphore;

int main()
{
	int pid;
	int status;
    int key = 0x1234;
	int args[3];
	int i; 
	srand(getpid());
	semaphore = createsem(key,1);

	for(i=0;i<3;i++){
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	
	for(i=0;i<3;i++)
		pid=wait(&status);

    erasesem(semaphore);
}

void proceso(int i){
	int k; 
	for(k=0;k<CICLOS;k++){
		semwait(semaphore);
		printf("Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf(" -- %s Sale\n", pais[i]);
		semsignal(semaphore);
		sleep(rand()%3);
	}
	exit(0); //Termina el proceso
}
