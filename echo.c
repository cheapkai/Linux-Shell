#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int echo(char **argarr,int cnt) 
{
  if(cnt<=1) {
    printf("\n");
    return 1;
  }
  for (int i = 1; i < cnt; i++)
  {
  	if(argarr[i][0]=='$')
  	{
  		const char* s = getenv(argarr[i]+1);
  		if(s!=NULL) printf("%s ",s);
  	}
  	else
  	{
	    int j;
	    for(j=0;argarr[i][j]!='\0';j++)
	    {
	      if(argarr[i][j]!='"') printf("%c",argarr[i][j]);
	    }

        printf(" ");
	}
  }
    printf("\n");
  return 1;
}
