#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void unsetenvv(char **argarr,int cnt){

  if(cnt==1)
  {
    printf("Arguments expected: >= 1 , given: 0");
  }
  else
  {
   // printf("%s\n",argarr[1]);
    unsetenv(argarr[1]);
  }
  return;
}
void setenvv(char **argarr, int cnt)
{
  if(cnt==0)
  {
    printf("Arguments expected: {1,2} , given: 0");
  }
  else if(cnt>3)
  {
    printf("Too many arguments");
  }
  else
  {
    if(cnt==2)
    {
      setenv(argarr[1] , " " , 2);
    }
    else
    {
      setenv(argarr[1] , argarr[2] , 2);
    }
  }
  return;
}