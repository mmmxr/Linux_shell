#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int argc=0;
char *argv[8];
void do_shell()
{
    //先创建子进程，对返回值进行判断
    pid_t id=fork();
    if(id == -1)
    {
        perror("fork");
        return;
    }
    else if(id > 0)
    {
        wait(NULL);//father wait!
    }
    else
    {
        //child do another thing!
        int i=0;
        int flag=0;
        for(;argv[i]!=NULL;i++)
        {
            if(strcmp(argv[i],"<")==0)
            {
                flag=1;
                break;
            }
            if(strcmp(argv[i],">")==0)
            {
                flag=2;
                break;
            }
        }

        if(flag==1)//input<
        {
            //printf("%d\n",flag);
            //printf("%s\n",argv[i+1]);
            argv[i]=NULL;
            int oldfd = open(argv[i+1],O_RDONLY,0644);

            if(oldfd<0)
            {
                perror("open");
                exit(3);
            }
            close(0);
            int newfd=dup(oldfd);
            if(newfd<0)
            {
                perror("dup");
                exit(4);
            }

            int ret = execvp(argv[0],argv);
            if(ret==-1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
        if(flag==2)//output >
        {
            argv[i]=NULL;
            int oldfd=open(argv[i+1],O_WRONLY | O_CREAT,0644);

            if(oldfd<0)
            {
                perror("open");
                exit(3);
            }
            close(1);
            int newfd=dup(oldfd);
            if(newfd<0)
            {
                perror("dup");
                exit(4);
            } 
            int ret = execvp(argv[0],argv);
            if(ret==-1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }

        }

        int ret = execvp(argv[0],argv);
        if(ret==-1)
        {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

}

void do_parse(char* buf)
{
    //过滤空白符的方法
    argc=0;//清空argc
    int i=0;
    int status=0;
    for(;buf[i]!=0;i++)
    {
        if(status==0&&buf[i]!=' ')
        {
            argv[argc++]=buf+i;
            status=1;
        }
        else if(buf[i]==' ')
        {
            status=0;
            buf[i]=0;
        }
    }
    argv[argc]=NULL;
}
int main()
{
    while(1)
    {
        char buf[64]={};
        printf("[myshell # ]");
        scanf("%[^\n]%*c",buf);
        do_parse(buf);
        do_shell();
    }
    return 0;
}
