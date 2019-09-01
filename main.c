#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include "headers.h"

typedef struct 
{
  int id;
  char name[100];
} Proc;

int proc_count;

Proc proc_arr[100];

void getprompt()
  {
    char a[200],*user;
    user=getenv("LOGNAME");
    gethostname(a,200);
    char *c,fi[100];
    c=getcwd(NULL,0);

    fprintf(stderr,"<%s%s@%s%s:%s%s%s>","\x1B[1;34m",user,a,"\x1B[0m","\x1B[1;32m",c,"\x1B[0m");
    return;

  }
 void sig_handler(int signo)
  {
    if(signo==SIGINT)
    {
      fprintf(stderr,"\n");
      getprompt();
    }

  }
 
void loop()
{
  char home[1005]; //contains the home directory (where the shell originates)
  getcwd(home,1000);
  int homepathlen=strlen(home);
  
   signal (SIGINT, SIG_IGN);
    signal (SIGQUIT, SIG_IGN);
    signal (SIGTSTP, SIG_IGN);
  while(1)
  {
    int i;

    int stat;
    while(waitpid(-1, &stat, WNOHANG) > 0);

    Proc temp[100];
    int tempcount=0;

    for(i=1;i<=proc_count;i++)
    {
      int a;
      if(a = kill(proc_arr[i].id,0)==-1 && errno==ESRCH)
      {
        printf("Process %s exited whose id is %d\n",proc_arr[i].name,proc_arr[i].id);
      }
      else
      { 
        //printf("a is %d\n", a);
        tempcount++;
        temp[tempcount]=proc_arr[i];
      }
    }

    proc_count = tempcount;
    for(i=1;i<=proc_count;i++)
    {
      proc_arr[i]=temp[i];
    }


    // Gets the current directory
   
    char presentdir[1005] = {'\0'};
    getcwd(presentdir,1000);

    int pwdlen=strlen(presentdir);
    int flag=0;
    char path[1005];

    // This code is to check if the directory which we are in right now is ahead of the directory in which shell is there or not.
    for(i=0;i<homepathlen;i++)
    {
      if(presentdir[i]!=home[i])
      {
      	flag=1;
      	break;
      }
    }

    if(flag==0)
    {
      //Copy the path ahead of the directory the shell is located in as shell's directory is supposed to be the home directory/
      for(i=homepathlen;i<=pwdlen;i++) path[i-homepathlen+1]=presentdir[i];
      path[0]='~';
    }

    else
    {
      //Specify the absolute path if the pwd is no ahead of the directory in which shell is there.
      for(i=0;i<=pwdlen;i++) path[i]=presentdir[i];
    } 
  
    char *username=getenv("USER"); //For display purpose, gets the valueof user environment variable
    char host[105];
    gethostname(host,100);
    printf("<%s%s@%s%s:%s%s%s>","\x1B[1;34m",username,host,"\x1B[0m","\x1B[1;32m",path,"\x1B[0m"); //Prompt printing

    char * line = readline();

    // Separates multiple command seperated by ";"
    char ** commands=split_commands(line,';');

    for(i=0; commands[i]!=NULL; i++)
    {
      if(strcmp(*(commands + i),"quit")==0)
      {
        printf("a\n");
        exit(0);
      }
      int type=0;
      int parr[2];
      int pflag=0;
      int fd=0;
      int j;
      pid_t newproc;
      char **pipecommand=split_commands(commands[i],'|');
      if(pipecommand[1]!=NULL) pflag=1;
      if(pflag==1)
      {

        for(j=0;pipecommand[j]!=NULL;j++)
        {
            char *argarr[40];      // Parses it which means splits the commands into arguments, argarr is the array of arguments, type is for foreground or background processes
            int cnt = parse(pipecommand[j],argarr,&type); // cnt is the number of arguments
            pipe(parr);
            newproc=fork();
            if(newproc==0)
            {
              dup2(fd , 0);
              if(pipecommand[j+1]!=NULL) dup2(parr[1],1);
              close(parr[0]);
              execute(argarr,type,cnt,home,homepathlen);
              exit(2);
            }
            else
            {

                  wait(NULL);
                  close(parr[1]);
                  fd = parr[0];
                  i++;
            }
        }
      }
      else{
      char *argarr[40];      // Parses it which means splits the commands into arguments, argarr is the array of arguments, type is for foreground or background processes
      
      int cnt = parse(commands[i],argarr,&type); // cnt is the number of arguments
      // Checks what the the command is, *argarr points to the first element in the argument array
      execute(argarr,type,cnt,home,homepathlen);
    }
    }



  }
}
int main()
{
  // Calls the infinite loop
  loop();
  return 0;
}

