#include <stdio.h>
#include <unistd.h>
#include <string.h>

int cd(char **argarr, int cnt, char * home)
{
  char directory[1005];

  if(cnt<=1)
  {
    chdir(home);
    return 1;
  }

  strcpy(directory,argarr[1]);

  if(directory[0]=='~')
  {
  	int i;
	char temp[1005];

    for(i=1; directory[i]!='\0'; i++) temp[i-1]=directory[i];

    int appendlen=i-1;

    for(i=0;home[i]!='\0';i++)
    {
      directory[i]=home[i];
    }

    int homelen = i;

    for(i=0;i<appendlen;i++)
    {
      directory[homelen+i]=temp[i];
    }

    directory[homelen+appendlen]='\0';
  } 

  if(directory[0]!='\0')
  {
    if(chdir(directory)!=0)
    {
      char errstring[1005];
      strcpy(errstring, "bash: cd: ");
	  strcat(errstring, directory);
      perror(errstring);
    }
  }
  return 1;
}
