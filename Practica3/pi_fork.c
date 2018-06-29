#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>

#define CHILDS 4
#define ITERATIONS 2000000000

double *pi_array[CHILDS] = {0};
double PI = 0;
void PIcalculation(void *args){
    int nchild = *((int *)args);
    int iterationStart = (nchild)*(ITERATIONS/CHILDS);
    int iterationEnd = (nchild+1)*(ITERATIONS/CHILDS);
    double sum = 0.0;
    signed short toggle = 0;
    int i;

    for(i = iterationStart; i < iterationEnd; i++){
        toggle = !toggle;
        sum = (double)((toggle*2)-1)/((2*i) + 1);
        *pi_array[nchild] = *pi_array[nchild] + sum;
    }

}

int main(){
    pid_t pid[CHILDS];
    int arrayArg[CHILDS];
    int status[CHILDS];
    int shmid[CHILDS];
    int i, j;
    long long startTime;
    long long endTime;
    float elapsed_time;
    struct timeval ts;

    for(i = 0; i < CHILDS; i++){
        shmid[i] = shmget(i, sizeof(pi_array), 0644 | IPC_CREAT); //Reserve memory location
        pi_array[i] = shmat(shmid[i], (void *)0,0);
    }

    gettimeofday(&ts, NULL);
    startTime = ts.tv_sec * 1000000 + ts.tv_usec; //Start

    for(i = 0; i<CHILDS; i++){
        pid[i] = fork();
        if(0 == pid[i]){
            arrayArg[i] = i;
            PIcalculation(&arrayArg[i]);
            exit(0);
        }
    }

    for(i = 0; i<CHILDS; i++)
        wait(&status[i]);
    
    gettimeofday(&ts, NULL);
    endTime = ts.tv_sec * 1000000 + ts.tv_usec; //End

    PI = 4*(*pi_array[0] + *pi_array[1] + *pi_array[2] + *pi_array[3]); //Extract the value of pi
    elapsed_time = (float)(endTime - startTime)/1000000.0;
    printf("pi is equal to %1.10f\n",PI);
    printf("Elapsed time = %2.2f seconds\n",elapsed_time);

    for(i = 0; i < CHILDS; i++){
        shmctl(shmid[i], IPC_RMID, NULL); //Remove shared memory
    }

}