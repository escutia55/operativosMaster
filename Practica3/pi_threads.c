#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define NTHREADS 4 //Number of Threads
#define ITERATIONS 2000000000

double pi_array[NTHREADS] = {0};
double PI;
void *PIcalculation(void *args){
    int nchild = *((int *)args);
    int iterationStart = (nchild)*(ITERATIONS/NTHREADS);
    int iterationEnd = (nchild+1)*(ITERATIONS/NTHREADS);
    double sum = 0.0;
    double pi = 0;
    signed short toggle = 0;
    int i;

    for(i = iterationStart; i < iterationEnd; i++){
        toggle = !toggle;
        sum = (double)((toggle*2)-1)/((2*i) + 1);
        pi = pi + sum;
    }
    pi_array[nchild] = pi;

}

//TODO: Computing takes way too long
int main(){
    int arrayArg[NTHREADS];
    int i = 0;
    long long startTime;
    long long endTime;
    float elapsed_time;
    struct timeval ts;
    pthread_t tid[NTHREADS];

    gettimeofday(&ts, NULL);
    startTime = ts.tv_sec * 1000000 + ts.tv_usec; //Start

    for(i = 0; i<NTHREADS; i++){
        arrayArg[i] = i;
        pthread_create(&tid[i], NULL, PIcalculation, (void *)&arrayArg[i]);
    }

    for(i = 0; i<NTHREADS; i++)
        pthread_join(tid[i],NULL);

    gettimeofday(&ts, NULL);
    endTime = ts.tv_sec * 1000000 + ts.tv_usec; //End

    PI = 4*(pi_array[0] + pi_array[1] + pi_array[2] + pi_array[3]);
    elapsed_time = (float)(endTime - startTime)/1000000.0;
    printf("pi is equal to %1.10f\n",PI);
    printf("Elapsed time = %2.2f seconds\n",elapsed_time);

    return 0;
}