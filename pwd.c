#include <stdio.h>
#include <unistd.h>

int pwd()
{
  char pwdir[10001];
  getcwd(pwdir,sizeof(pwdir));
  printf("%s\n",pwdir);
  return 1;
}
