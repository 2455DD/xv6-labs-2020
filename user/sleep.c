#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#define STD_INPUT 0
#define STD_OUTPUT 1
#define STD_ERR 2


int 
main(int argc,char* argv[]){
    if(argc<1||argc>2){
         fprintf(STD_ERR, "usage: sleep <number>\n");
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}