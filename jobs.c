#include <stdio.h>

typedef struct 
{
  int id;
  char name[100];
} Proc;

extern int proc_count;

extern Proc proc_arr[100];

void jobs()
{
	int i;
	for(i=1;i<=proc_count;i++)
	{
	  printf("[%d] %s %d\n", i, proc_arr[i].name, proc_arr[i].id);
	}
}