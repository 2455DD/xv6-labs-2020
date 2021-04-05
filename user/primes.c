#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

#define STD_ERR 2

#define READEND 0
#define WRITEEND 1

typedef int pid;

void childProcess(int p2Left[2]){
    int p2Right[2];
    close(p2Left[WRITEEND]);   //Needn't write 2 left
    int prime;
    if((read(p2Left[READEND],&prime,sizeof(int))==0)){  //if your parent is not ready, BLOCK(shows that prime is set)
        //Stop expanding the Process Tree when Legnth of read num is 0 which means last prime is found
        close(p2Left[READEND]); //Release File Descriper
        exit(0);
    } 
    printf("prime %d\n",prime);
    pipe(p2Right);
    if(fork()==0){  //Start your right process, NOW! The block runs in child process
        close(p2Left[READEND]);    //I beilive your child don't need that, right?
        childProcess(p2Right);      
    }
    else{
        close(p2Right[READEND]);    //Needn't read from right
        int num;
        while(1){   //Endless Loop Condition must have a Break cond 
            if(read(p2Left[READEND],&num,sizeof(int))==0) {//END OF STREAM
                break;
            }
            if(num%prime!=0){   //Doesn't devided by ALL PRIME including prime of this process and the prime from left
                write(p2Right[WRITEEND],&num,sizeof(int));
            }   
        }
        close(p2Right[WRITEEND]);   // let your child know, you are right here waiting for it
        close(p2Left[READEND]); //Release
        wait((int*)0);
    }
    exit(0);
}








int 
main(int argc,char* argv[]){
    int p[2];
    pipe(p);
    int i;          //Loop Variable, sent as generating number
    if(fork()==0){  //Not ROOT PROCESS
        childProcess(p);    //Calling function to decrease the length of main func( and other details). Passing Left Side Pipe
    }
    else{           //ROOT PROCESS
        close(p[READEND]);    //No need to read from right

        for(i=2;i<=35;i++){
            write(p[WRITEEND],&i,sizeof(int));     //Official Hint: passing int is better than passing by ASCII I/O 
        }
        close(p[WRITEEND]);    //No need to write, just wait for child to exit
        wait((int*)0);  //Wait, just wait for them
    }
    exit(0);
}