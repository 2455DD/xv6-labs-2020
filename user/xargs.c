#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define STD_INPUT 0

int main(int argc, char *argv[]){
    char* parameters[MAXARG];
    char line[1024];
    int read_len,arg_index=0;
    int i;
    
    //找到指令位置，并将参数填充进parameters内
    char* command=argv[1];
    for(i=1;i<argc;i++)
        parameters[arg_index++]=argv[i];

    //开始逐行读取
    while((read_len=read(STD_INPUT,line,1024))>0){
        if(fork()==0){  //子进程
            char *argu=(char*)malloc(sizeof(line));
            int index=0;
            // 将这一行的所有参数压入parameters队尾
            for(i=0;i<read_len;i++){
                if(line[i]==' '||line[i]=='\n'){    //若有空格或转行
                    argu[index]=0;//截断字符串
                    //将截断的一个完整参数排进参数表中
                    parameters[arg_index++]=argu;
                    index=0;
                    argu=(char*)malloc(sizeof(line));
                }
                else
                    argu[index++]=line[i];
            }
            argu[index]=0;
            parameters[arg_index]=0;
            exec(command,parameters);
        }
        else wait(0);   //父进程等待
    }
    exit(0);
}