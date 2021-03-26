#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
//#include<readline/readline.h>
//#include<readline/history.h>
//#include<cstdlib>
char shell_dir[1000];
int cntback=0;
int backpid[10010];
char backproc[1000][1000];
int bgflag[1000];
int globalid;
void piping(char** argv);
void ioredirect(char** argv);
void jobs();
void kjob(char** argv);
void overkill();
void setenvfunction(char **args);
void unsetenvfunction(char **args);
void getenvfunction(char **args);
void checkjob();
void cronjob(char** argv);
void controlz(int signum);
void echo(char** argv);
void controlc(int signum);
void pwd();
void cd(char* req_path);
void ls(char** argv);
void pinfo_general();
void pinfo_pid(char* pid);
int runfg(char** argv);
void fg(char** argv);
int runbg(char** argv);
void bg(char** argv);
void abs_to_rel();
char** split_line(char* cmdline);
char** split_attributes(char* cmd);
int execute_command(char ** argv);