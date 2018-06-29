#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#define atomic_xchg(A,B) __asm__ __volatile__( \
" lock xchg %1,%0 ;\n" \
: "=ir" (A) \
: "m" (B), "ir" (A) \
);
#define CICLOS 10

typedef struct _QUEUE{
	int elements[CICLOS];
	int head; 
	int tail;
} QUEUE;

typedef struct semaphore{
	int contador;
	QUEUE bloqueados;
} semaphore;


void _enqueue(QUEUE *q,int val);
int _dequeue(QUEUE *q);
void waitsem(semaphore* s);
void signalsem(semaphore* s);
void initsem(semaphore* s, int init);
void proceso(int i);

char *pais[3] = {"Peru","Bolivia","Colombia"};
semaphore* sem;
int g;

int main()
{
	int pid;
	int status;
	int shmid;
	int args[3];
	int i; 
	
	//Solicitar memoria compartida
	shmid=shmget(0x1234, sizeof(sem), 0666|IPC_CREAT);
	if (shmid==1)
	{
		perror("Error en la memoria compartida\n");
		exit(1);
	}
	// Conectar la variable a la memoria compartida
	sem = shmat(shmid,NULL,0);

	if(sem==NULL){
		perror("Error en el shmat\n");
		exit(2);
	}
	initsem(sem,1);
	srand(getpid());
	for(i=0;i<3;i++){
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	
	for(i=0;i<3;i++)
		pid=wait(&status);

	shmdt(sem);
}

void _enqueue(QUEUE *q,int val)
{
	q->elements[q->head]=val;
	q->head++;
	q->head=q->head%CICLOS;
}


int _dequeue(QUEUE *q)
{
	int valret;
	valret=q->elements[q->tail];
	q->tail++;
	q->tail=q->tail%CICLOS;
	return(valret);
}

void waitsem(semaphore* s){
	int l=1;
	do{atomic_xchg(l,g); } while(l!=0);
	s->contador--;
	g=0; //Se "libera" la g 
	if (s->contador<0){ //El primer proceso no se bloquea 
		//Los procesos que no puedan "tomar" el semaforo se bloquean y se mandan a la cola de bloqueados
		_enqueue(&s->bloqueados, getpid());
		kill(getpid(),SIGSTOP);
	}
}

void signalsem(semaphore* s){
	int pid;
	int l=1;
	do { atomic_xchg(l,g); } while(l!=0); //Espera a hacer el xchg
	s->contador++;
	g = 0; 
  if (s->contador <=0){
	  	//Cuando un proceso sale de la seccion critica, el siguiente proceso en la cola de bloquedos se ejecuta
		pid = _dequeue(&s->bloqueados);
		kill(pid,SIGCONT);
  }
}

void initsem(semaphore* s, int init){
	s->bloqueados.tail = 0;
	s->bloqueados.head = 0;
	s->contador = init;
	g = 0;
}


void proceso(int i){
	int k;
	for(k=0;k<CICLOS;k++){
		waitsem(sem);
		printf("Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf(" -- %s Sale\n", pais[i]);
		signalsem(sem);
		sleep(rand()%3);
	}
	exit(0); //Termina el proceso
}
