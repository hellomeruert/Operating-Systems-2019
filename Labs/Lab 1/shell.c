#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_LINE 80

char hist[10][MAX_LINE];
int numCommands = 0;


void showHist(){
    int i;
    int j = 0;
    int histCount = numCommands;
    for (i = 0; i<10; i++){
       	printf("%d.  ", histCount);
        while (hist[i][j] != '\n' && hist[i][j] != '\0'){
            printf("%c", hist[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        histCount--;
        if (histCount ==  0)
        break;
    }
}

void readTok(char in[], char *args[],int *isAmp)
{
    int charInd = 0;
	int length;
    int next;

    
    length = read(STDIN_FILENO, in, MAX_LINE);

    
    next = -1;
    if (length == 0)
        exit(0);
    if (length < 0){
        perror("could not read command");
	exit(-1);
    }

    
    for (int i = 0; i < length; i++) {
        switch (in[i]){
        case ' ':
        case '\t' :
            if(next != -1){
                args[charInd] = &in[next];
                charInd++;
            }
            in[i] = '\0';
            next = -1;
            break;

        case '\n':
            if (next != -1){
                args[charInd] = &in[next];
                charInd++;
            }
            in[i] = '\0';
            args[charInd] = NULL;
            break;

        case '&':
            *isAmp = 1;
            in[i] = '\0';
            break;

        default :
            if (next == -1)
                next = i;
	}
    }
    args[charInd] = NULL;

    /
    if(strcmp(args[0],"history") == 0){
            	if(numCommands>0){
            		showHist();
    		} else{
    			printf("\nNo Commands in history\n");
    			}
    }
    
    	else if (**args == '!'){
    		int check2 = args[0][1]- '0';
    		int check3 = args[0][2];
    		if(check2 > numCommands){
    		printf("\nCommand not found\n");
    		strcpy(in,"Invalid Command");
    		}else if (check3 != 0){
    				printf("\nInput too large.  History only contains last 10 commands. \n");
    				strcpy(in,"Invalid Command");
    		}	else{
    				if(check2==-15){/
    					 strcpy(in,hist[0]);

    				}	else if(check2 == 0){
    						printf("Can not enter 0. Enter ! followed by single integer 1-9");
    						strcpy(in,"Invalid Command");
    					}	else if(check2 >= 1){
    							strcpy(in,hist[numCommands-check2]);
    						}

    				}
    		}
    
	
    for (int i = 9; i>0; i--)
    	strcpy(hist[i], hist[i-1]);
        strcpy(hist[0],in);
        numCommands++;
    	if(numCommands>10){
    	numCommands=10;
    	}
}


int cmdLoop(void){
    char in[MAX_LINE];
    char *args[MAX_LINE/2+1];
    int isAmp;
    pid_t pid;
    int loops;
    int continueLoop = 1;

    while (continueLoop){
	isAmp = 0;
	printf("osh>");
    fflush(stdout);
    readTok(in, args, &isAmp);   
    pid = fork();

   	if (pid < 0) {
   		printf("Fork failed.\n");
   		exit (1);
 	} 		else if (pid == 0){
 		 				if (execvp(args[0], args) == -1 && strcmp(in, "history") != 0)
 		 				{
 		 					printf("Invalid Command (ctrl^c to exit)\n");
 		 				}
    		}	else{
    			loops++;
    				if (isAmp == 0){
    					loops++;
    					wait(NULL);
    				}

    		}
	}
	return 0;
}


int main(void){
cmdLoop();
return 0;
}
