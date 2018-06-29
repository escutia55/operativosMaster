//=================================================
// getty.c
// Description: This program does the login access. Asks the user 
// for username and password and executes sh if login is successfull
// Authors: Luis Fernando Moreno Alonzo (ie693314)
//          Ricardo Escutia Ruiz        (ie693075)
//=================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define WORD_LENGTH 30

struct user_info{
    char *username;
    char *password;
};

typedef enum{
    Fail, 
    Success
}bool;

void checkLoginAccess();

void checkLoginAccess(){
    char *file_input;
    char user_input[WORD_LENGTH] = {0};
    char *split;
    struct user_info user;
    bool login_access = Fail;
    size_t len = 0;
    FILE * file;

    file = fopen("passwd.txt", "r");

    if(file){
        while(Fail == login_access){
            file = fopen("passwd.txt", "r");  
            printf("Username: ");
            scanf("%s",(char *)&user_input); 
            
            //========= Check user name ===================
            while ((getline(&file_input, &len, file)) != -1) { //Start reading line by line 
                split = strtok(file_input,":");
                user.username = split; //Get the user name
                split = strtok(NULL,":"); 
                split = strtok(split,"\n"); //Delete the end of line from password: \n
                user.password = split; // Get the password

                    if(0 == strcmp(user.username, user_input)){ //Compare given username with username from file
                        login_access = Success; //Correct user name, go to password check
                        break;
                    } 
                }

            //========= Check password ===================
            if(Fail == login_access){ 
                printf("wrong user name\n");
                    file = fopen("passwd.txt", "r");
            }else{
                printf("password: ");
                scanf("%s", (char *)&user_input);     
                if(0 == strcmp(user.password, user_input)){ //Compare given password and password from file
                    printf("correct!");
                    execlp("./sh", "./sh", NULL); //If password was typed succesfully, begin sh process
                }else{
                    printf("wrong password\n");
                    login_access = Fail;
                    fclose(file);
                    }
                }
        }

        fclose(file);

    }else{ 
        printf("Passwd file does not exist! \n");
    }
}

int main(){
    checkLoginAccess();
    return 0;
}