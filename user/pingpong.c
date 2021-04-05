#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"

#define STD_ERR 2

#define READEND 0
#define WRITEEND 1

typedef int pid;

int 
main(int argc,char* argv[]){
    if(argc>0){
        fprintf(STD_ERR,"usage:pingpong\n");
    }
    char buf[10]; 
    int p[2];           //Parent-Child Pipe
    pipe(p);
    pid childPID=fork();
    if(childPID<0){
        fprintf(STD_ERR,"Fork Error,Exit\n");
        exit(1);
    }
    else if(childPID==0){       //Child Process
        read(p[READEND],buf,1);
        printf("%d: received ping\n",getpid(),buf);
        close(p[READEND]);
        write(p[WRITEEND],"0",1);
        close(p[WRITEEND]);
    }
    else{               //Parent Process
        write(p[WRITEEND],"0",1);
        close(p[WRITEEND]);
        wait(0);    //ESSENTIAL!
        read(p[READEND],buf,1);
        close(p[READEND]);  //
        printf("%d: received pong\n",getpid());
    }

    exit(0);
}