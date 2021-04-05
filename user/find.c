#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


#define STD_ERR 2


void find(char *path,char* target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if((fd = open(path, 0)) < 0){
    fprintf(STD_ERR, "find: cannot open %s\n", path);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(STD_ERR, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

    //searching for every stuff in the dir
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        //adding / to the tail of the path string
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
      //invalid entry: no actual file
      if(de.inum == 0){
        continue;
      }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
    switch(st.type){
        case T_FILE:
            if(strcmp(de.name,target)==0)printf("%s\n",buf);
            break;

        case T_DIR:
            if(strlen(buf) + 1 + DIRSIZ + 1 > sizeof buf){
                fprintf(STD_ERR,"find: path too long\n");
            break;
            }   
            //if the stuff is . or .. ,skip it
            if(strcmp(de.name,".")!=0&&strcmp(de.name,"..")!=0)find(buf,target);
            break;
            }
    
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if(argc > 3){
    fprintf(STD_ERR,"usage:find <path> <target>\n");
    exit(1);
  }
  find(argv[1],argv[2]);
  exit(0);
}
