char *readline();
int parse(char *ip, char *args[],int *type);
char** split_commands(char* ip, char delim);
int cd(char **argarr, int cnt, char * home);
int echo(char **argarr,int cnt);
int pwd();
void pinfo(char **argarr, int cnt, char *home, int homepathlength);
void ls(char **argarr,int cnt);
int execute(char ** argarr, int type, int cnt, char* home, int homepathlen);
void unsetenvv(char ** argarr,int cnt);
void setenvv(char **argarr, int cnt);
void jobs();
void overkill();
void kjob(char ** argarr, int cnt);
