#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>

#define CICLOS 10

typedef struct {
	long msg_type;		// Tipo de mensaje, debe ser long
	char mensaje[100];	// Contenido
} MSGTYPE; 

char *pais[3] = {"Peru","Bolivia","Colombia"};

int msgqid;	// Identificador de la cola de mensajes

void emisor(){
	int i;
	MSGTYPE m;
	m.msg_type=1;
	strcpy(m.mensaje,"msg");
	msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);

	return;
}

void receptor(){
	MSGTYPE m;	// Donde voy a recibir el mensaje
	int retval;

	retval=msgrcv(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),1,0);
	if(retval<0)
        printf("Error al recibir el mensaje\n");
	return;
}

void proceso(int i){
	int k; 
	for(k=0;k<CICLOS;k++){
		receptor();
		printf("Entra %s ", pais[i]);
		fflush(stdout);
		sleep(rand()%3);
		printf(" -- %s Sale\n", pais[i]);
		emisor();
		sleep(rand()%3);
	}
	exit(0); //Termina el proceso
}

int main()
{
	int pid;
	int status;
	int args[3];
	int i; 
	
	srand(getpid());
	// Crear un buzón o cola de mensajes
	msgqid=msgget(0x1234,0666|IPC_CREAT);

	if(msgqid < 0)
    	printf("Error creando la cola de mensajes");

	emisor(); //Mandar un mensaje

	for(i=0;i<3;i++){
		pid=fork();
		if(pid==0)
			proceso(i);
	}
	
	for(i=0;i<3;i++)
		pid=wait(&status);

	msgctl(msgqid,IPC_RMID,NULL);
}
