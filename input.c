#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *readline()
{
  char *buffer;
  size_t bufsize = 0; //getline() allocates a buffer for us
  getline(&buffer, &bufsize, stdin);
  return buffer;
}

int parse(char *ip, char *args[],int *type)
{
    int count = 0;
    char *pointer = ip;
    while(*pointer != '\0')   {
      if(*pointer!=' ' && *pointer!='&')
    { 
      *args = pointer;
      count++;
      args++;
    }
      while(*pointer != ' ' && *pointer != '\t' && *pointer != '\0' && *pointer != '\n')
      {
        if (*pointer == '&') *type = 44;
        pointer++;
      }
      while(*pointer == ' ' || *pointer == '\t' || *pointer == '\n')
      {
        *pointer = '\0';
        pointer++;
      }
    }
    *args = '\0';
    return count;
}

char** split_commands(char* ip, char delim)
{
    char* temp=(char*)malloc((1001)*sizeof(char));
    strcpy(temp,ip);
    char **tokens = malloc(100 * sizeof(char*));
    char *tmp;
    int i=0;
    tmp = strtok(temp, &delim);
    while (tmp != NULL) 
    {
      tokens[i] = tmp;
      i++;
      tmp = strtok(NULL,&delim);
    }
    tokens[i] = NULL;
    return tokens;
}
