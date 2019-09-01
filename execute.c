#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "headers.h"

typedef struct 
{
  int id;
  char name[100];
} Proc;

extern int proc_count;

extern Proc proc_arr[100];


int execute(char ** argarr, int type, int cnt, char* home, int homepathlen)
{
  if(cnt==0) return 0;

  if(strcmp(*argarr, "quit") == 0)
  {
    exit(0);
  }

  int status;
  pid_t pid, wpid;

  pid = fork();

  if (pid == 0)
  {
    if(argarr[0][0]!='\0')
    {
    int inflag=0, outcount=0, appendcount=0;
    char inpfile[64], outputfile[16][64], appendfile[16][64];
    int fd;

    int i;
    for(i=0;argarr[i]!='\0';i++)
    {
      if(strcmp(argarr[i],"<")==0)
      {        
        argarr[i]=NULL;
        strcpy(inpfile,argarr[i+1]);
        inflag=1;           
      }
      else if(strcmp(argarr[i],">")==0)
      {      
        argarr[i]=NULL;
        strcpy(outputfile[outcount++],argarr[i+1]);
      }         
      else if(strcmp(argarr[i],">>")==0)
      {      
        argarr[i]=NULL;
        strcpy(appendfile[appendcount++],argarr[i+1]);
      }  
    }

    if(inflag!=0)
    { 
      fd = open(inpfile, O_RDONLY, 0);
      if(fd<0) perror("Error opening input file");
      if(dup2(fd,0) < 0) ("Error - input duping");
      close(fd);
    }

    for (i = 0; i < outcount; ++i)
    {
      fd = open(outputfile[i], O_WRONLY | O_TRUNC | O_CREAT ,0644);
      if(fd<0) perror("Error opening output file");
      if(dup2(fd,STDOUT_FILENO)<0) perror("Error - output duping");
      close(fd);
    }

    for (i = 0; i < appendcount; ++i)
    {
      fd = open(appendfile[i], O_WRONLY | O_APPEND | O_CREAT, 0644);
      if(fd<0) perror("Error opening the output(append) file");
      if(dup2(fd,STDOUT_FILENO)<0) perror("Error - output(append) duping");
      close(fd);
    }
 
    if(strcmp(*argarr, "pwd") == 0)
    {
      pwd();
    }
    else if(strcmp(*argarr, "cd") == 0)
    {
      cd(argarr,cnt,home);
    }
    else if(strcmp(*argarr, "echo") == 0)
    {
      echo(argarr,cnt);
    }
    else if(strcmp(*argarr, "unsetenv")==0)
    {
      unsetenvv(argarr,cnt);
    }
    else if(strcmp(*argarr, "pinfo") == 0)
    {
      pinfo(argarr,cnt,home,homepathlen);
    }
    else if(strcmp(*argarr, "ls") == 0)
    {
      ls(argarr,cnt);
    }
    else if(strcmp(*argarr, "setenv") == 0)
    {
      setenvv(argarr,cnt);
    }
    else if(strcmp(*argarr, "jobs") == 0)
    {
      jobs();
    }
    else if(strcmp(*argarr, "kjob") == 0)
    {
      kjob(argarr,cnt);
    }
    else if(strcmp(*argarr, "overkill") == 0)
    {
      overkill();
    }
    else
    {
      if(execvp(argarr[0], argarr) == -1)
      {
        printf("%s: command not found\n",argarr[0]);
      }
    }
    //printf("execution complete\n");
    }
    
    exit(EXIT_FAILURE);
  }

  else if (pid < 0) 
  {
    perror("Error");
  } 

  else
  {

    if(type==0)
    {
      do
      {
        wpid = waitpid(pid, &status, WUNTRACED);
      }
      while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    else
    {
      //background process is stored into the array
      proc_count++; //note that proc_arr is 1 indexed
      proc_arr[proc_count].id = (int) pid;
      strcpy(proc_arr[proc_count].name,argarr[0]);
    }

  }

  return 0;
}
