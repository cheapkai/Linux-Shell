#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void ls(char **argarr,int cnt)
{

	void longlist(char *directory, char *filename)
	{
		char path[200];
		strcpy(path,directory);
		strcat(path,"/");
		strcat(path,filename);

		struct stat fileStat;

		if(stat(path,&fileStat)<0)
		{
			perror("Error");
			return;
		}

		struct passwd * uname = getpwuid(fileStat.st_uid);
		struct group * ugroup = getgrgid(fileStat.st_gid);

		struct tm * time = localtime(&fileStat.st_mtime);

		printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

	    printf(" %2lu", fileStat.st_nlink);

	    printf(" %s", uname->pw_name);
	    printf(" %s", ugroup->gr_name);

	    printf(" %5ld",fileStat.st_size);

		char string[100];
		strftime(string,10,"%b",time);

	   	printf(" %4d %s %2d",time->tm_year + 1900, string, time->tm_mday);
		printf(" %02d:%02d",time->tm_hour, time->tm_min);
	    
	    if (S_ISDIR(fileStat.st_mode)) printf(" %s%s%s\n", "\x1B[1;34m", filename, "\x1B[0m");
	    else if (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) printf(" %s%s%s\n", "\x1B[1;32m", filename, "\x1B[0m");
	    else printf(" %s\n", filename);
	}

	int lflag = 0;
	int aflag = 0;

	int dirs[1000];
	int dircount=0;

	int i,j;
	for(i=1; i<cnt; i++)
	{
		if(argarr[i][0]=='-')
		{
			if(strcmp(argarr[i],"-l")==0) lflag=1; 
			else if(strcmp(argarr[i],"-a")==0) aflag=1;
			else if(strcmp(argarr[i],"-la")==0 || strcmp(argarr[i],"-al")==0)
			{
				aflag=1;
				lflag=1;
			}
		}

		else
		{
			dirs[dircount++]=i;
		}
	}

 	int hide(const struct dirent *name)
	{
    	if(name->d_name[0] == '.') return 0;
    	else return 1;
	}

	int namecmp(const char *s1, const char *s2)
	{
		if(s1[0]=='.') s1++;
		if(s2[0]=='.') s2++;

		return strcmp(s1,s2);
	}

	int hiddensort(const struct dirent **name1, const struct dirent **name2)
	{
		return namecmp((*name1)->d_name, (*name2)->d_name);
	}

	int noarg=0;

	if(dircount==0)
	{
		argarr[cnt] = malloc(2*sizeof(char));
		argarr[cnt][0]='.';
		argarr[cnt][1]='\0';
		dirs[dircount++]=cnt;
		cnt++;
		noarg=1;
	}

	for(i=0;i<dircount;i++)
	{
		char directory[1005];
		strcpy(directory,argarr[dirs[i]]);

		struct dirent **namelist;
		int ret;
		if(aflag) ret = scandir(directory, &namelist, NULL, hiddensort);
		else ret = scandir(directory, &namelist, hide, alphasort);

		if(ret<0) perror("Error");
		else
		{
			printf("%s:\n",directory);
			if (lflag==0)
			{
				for(j=0;j<ret;j++)
				{
					char filename[100];
					strcpy(filename,namelist[j]->d_name);

					char path[200];
					strcpy(path,directory);
					strcat(path,"/");
					strcat(path,filename);

					struct stat fileStat;

					if(stat(path,&fileStat)<0)
					{
						perror("Error");
						continue;
					}

				    if (S_ISDIR(fileStat.st_mode)) printf("%s%s%s  ", "\x1B[1;34m", filename, "\x1B[0m");
					else if (fileStat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) printf("%s%s%s  ", "\x1B[1;32m", filename, "\x1B[0m");
					else printf("%s  ", filename);
				}
			}

			else
			{
				for(j=0;j<ret;j++)
				{
					char filename[100];
					strcpy(filename,namelist[j]->d_name);
					longlist(directory,filename);
				}
			}

			if(lflag==0) printf("\n");
			if(i<dircount-1) printf("\n");

			free(namelist[i]);
		}
	}

	if(noarg) free(argarr[cnt-1]);

}
