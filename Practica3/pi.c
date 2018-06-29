#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#define ITERATIONS 2000000000

int main(){
    long long startTime;
    long long endTime;
    float elapsed_time;
    struct timeval ts;

    double pi = 0;
    double sum = 0.0;
    signed short toggle = 0;
    int i;

    gettimeofday(&ts, NULL);
    startTime = ts.tv_sec * 1000000 + ts.tv_usec; //Start

    for(i = 0; i < ITERATIONS; i++){
        toggle = !toggle;
        sum = (double)((toggle*2)-1)/((2*i) + 1);
        pi = pi + sum;
    }
    
    pi = pi*4;
    gettimeofday(&ts, NULL);
    endTime = ts.tv_sec * 1000000 + ts.tv_usec; //End

    elapsed_time = (float) (endTime - startTime)/1000000.0;
    printf("pi is equal to %1.10f\n",pi);
    printf("Elapsed time = %2.2f seconds\n",elapsed_time);
    return 0;

  }