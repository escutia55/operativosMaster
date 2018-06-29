//=================================================
// Sh.c
// Description: Shell file. This program interprates cmds given by the user
// Authors: Luis Fernando Moreno Alonzo (ie693314)
//          Ricardo Escutia Ruiz        (ie693075)
//=================================================
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

#define WORD_LENGTH 30

void shutdown();

void shutdown(){
    FILE * pid_file;
    char * file_input;
    pid_t pid;

    pid = fork();

    if(0 == pid){
        execlp("pkill", "pkill", "init", (char *)NULL); //Kill init processes
    }

    pid = fork();

    if(0 == pid){ 
        execlp("pkill", "pkill", "xterm", (char *)NULL); //Kill getty processes
    }

    remove("pid_file.txt");
}

void get_cmd(){
    char *split;
    char user_cmd[WORD_LENGTH] = ""; //Cmd provided by the user
    char *cmd[3] = {"Hi", "Bye","ok"}; //cmd array (this will be passed to execlp as three arguments)
    pid_t pid;

    FILE * pid_file;

    pid_file = fopen("pid_file.txt", "a+"); //File containing all pids
    fprintf(pid_file, "%d\n", getpid()); //Store the current pid
    fclose(pid_file);

    printf("sh >");
    while(0 != strcmp(user_cmd, "exit")){       
        scanf(" %[^\n]s",user_cmd);
        //Cut the given cmd into three chunks
        split = strtok(user_cmd," ");
        cmd[0] = split;
        split = strtok(NULL," ");
        cmd[1] = split;
        split = strtok(NULL," ");
        cmd[2] = split;
        pid = fork();

        if(0 == pid){
            if(0 == strcmp(user_cmd, "shutdown")){ //if shutdown was typed
			    shutdown();
                exit(0);
		    }else
                execlp(cmd[0], cmd[0], cmd[1], cmd[2], (char *)NULL); //Execute user cms
            }
            wait(0);  //This wait is to make sure the cmd ends before printing "the line below"
            printf("sh >");       
        }    
    }

int main(){

    get_cmd();

    return 0;
}