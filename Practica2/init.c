//=================================================
// init.c
// Description: This program creates 6 getty processes
// Authors: Luis Fernando Moreno Alonzo (ie693314)
//          Ricardo Escutia Ruiz        (ie693075)
//=================================================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define PROCESSES 6

void makeGetty();

void makeGetty(int process_count){
    int i;
    pid_t pid;
    FILE *pid_file;

    pid_file = fopen("pid_file.txt", "w+");
    fprintf(pid_file, "%d\n", getpid()); //Store the parent's pid
    fclose(pid_file);
    
    for(i = 0; i<process_count; i++){ //Create "process_count" processes
        pid = fork(); //Create a child
        if(0 == pid){
            pid_file = fopen("pid_file.txt", "a+");
            fprintf(pid_file, "%d\n", getpid()); //Store the childs pid (getty pid)
            fclose(pid_file);
            execlp("xterm", "xterm","-e","./getty", NULL);
        }
    }
}

int main(){
    int status;
    makeGetty(PROCESSES);

    while ((wait(&status)))
        makeGetty(1);//Create a process if closed

    return 0;
}
