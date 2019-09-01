#include <sys/types.h>
#include <signal.h>
#include <stdio.h>

typedef struct{
	int id;
	char name[100];
} Proc;

extern int proc_count;

extern Proc proc_arr[100];

void overkill()
{	
	for(int i=1;i<=proc_count;i++)
	{	
		kill(proc_arr[i].id,SIGKILL);
	}

}