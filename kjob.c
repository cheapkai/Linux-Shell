#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>


int power(int a, int b)
{
	int x = 1;
	int i;
	for (i = 0; i < b; ++i)
	{
		x*=a;
	}

	return x;
}


typedef struct{
	int id;
	char name[100];
} Proc;

extern int proc_count;

extern Proc proc_arr[100];

void kjob(char ** argarr, int cnt)
{
	if(cnt <= 2)
	{
		printf("Too few arguments!\n");
	}

	else if (cnt > 3)
	{
		printf("Too many arguments\n");
	}

	else
	{
		int job_id=0;
		for(int i=0;i<strlen(argarr[1]);i++)
		{
			job_id += (argarr[1][i]-'0')*power(10,i);
		}

		int sigval=0;
		for(int i=0;i<strlen(argarr[2]);i++)
		{
			sigval += (argarr[2][i]-'0')*power(10,i);
		}

		kill(proc_arr[job_id].id, sigval);
	}

	return;
}