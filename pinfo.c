#include <stdio.h>
#include <unistd.h>
#include <string.h>

void pinfo(char **argarr, int cnt, char *home, int homepathlength)
{
  char stat1[1000];
  char exec1[1000];
  if(cnt>1)
  {
    strcpy(stat1,"/proc/");
    strcat(stat1,argarr[1]);
    strcat(stat1,"/stat");
    strcpy(exec1,"/proc/");
    strcat(exec1,argarr[1]);
    strcat(exec1,"/exe");
  }
  else
  {
    sprintf(stat1,"/proc/%d/stat",getpid());      
    sprintf(exec1,"/proc/%d/exe",getpid());
  } 

  FILE *fp;
  if((fp=fopen(stat1,"r"))==NULL)
  {
    printf("Error: process doesn't exist\n");
  }
  else
  {
    char execname[1000];
    int pid;
    char runstatus;
    long unsigned int mem; 
    fscanf(fp,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid,&runstatus,&mem);                       
    fclose(fp);
    printf("Process ID -- %d\n",pid);
    printf("Process Status -- %c\n",runstatus);
    printf("Memory -- %lu\n",mem);

    int a = readlink(exec1,execname,256);
    execname[a]='\0';


    if(a>=homepathlength)
    {
      int inhome=1;
      int i;
      for(i=0; i<homepathlength; i++)
      {
        if (execname[i]!=home[i])
        {
          inhome=0;
          break;
        }
      }

      if(inhome)
      {
        char final[1000];
        final[0]='~';
        for(i=homepathlength; i<a; i++)
        {
          final[i-homepathlength+1] = execname[i];
        }
        final[i-homepathlength+1]='\0';
        strcpy(execname,final);
      }
 
    }

    printf("Executable Path -- %s\n",execname);
  }  
}

