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
#include "global.h"
//char shell_dir[1000];
char *read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;
}
void echo(char** argv)
{
	if(argv[1]==NULL)
	{
		return;
	}
	printf("%s\n",argv[1]);
	return;
}
void pwd()
{
	char buf[1000];
	char* p1=getcwd(buf,sizeof(buf));
	if(p1==NULL)
	{
		perror("getcwd error in pwd:");
	}
	printf("%s\n",buf);
}
void cd(char* req_path)
{
	if(req_path==NULL||strcmp(req_path,"&")==0)
	{
		chdir(shell_dir);
		return;
	}
	if(req_path[0]=='~')
	{
 //   	chdir(shell_dir);
		char newstr[1000];
		int len=strlen(req_path);
		for(int j=1;j<len;j++)
			newstr[j-1]=req_path[j];
		newstr[len-1]='\0';
		char temp[1000];
		strcpy(temp,shell_dir);
		strcat(temp,newstr);
		chdir(temp);
		return;
	}
	char curr_dir[1000];
	getcwd(curr_dir,sizeof(curr_dir));
	if(req_path[0]=='/')
		chdir(req_path);
	else
	{
		strcat(curr_dir,"/");
		strcat(curr_dir,req_path);
		chdir(curr_dir);
	}
	char buf[1000];
	char* p1=getcwd(buf,sizeof(buf));
	if(p1==NULL)
	{
		perror("getcwd error in cd");
		exit(1);
	}
//	printf("%s@%s:%s ",username,hostname,buf);

}
void ls(char** argv)
{
    char curr_dir[1000];
    char buf[1000];
    char* ran2=getcwd(curr_dir,sizeof(curr_dir));
    if(ran2==NULL)
	{
		perror("getcwd error in ls");
		exit(1);
	}
    DIR *mydir;
    struct stat stat1;
    struct dirent* hell;
    if(argv[1]==NULL||strcmp(argv[1],"&")==0)
	{
		mydir=opendir(curr_dir);
    	while((hell=readdir(mydir))!=NULL)
    	{
    		if(strcmp(hell->d_name,".")!=0&&strcmp(hell->d_name,"..")!=0)
    		printf("%s\n",hell->d_name);
    	}	
    	return;
	}
    if(strcmp(argv[1],"-a")==0)
    {
    	mydir=opendir(curr_dir);
    	while((hell=readdir(mydir))!=NULL)
    	{
    		printf("%s\n",hell->d_name);
    	}	
    	return;
    }
    if(strcmp(argv[1],"-l")==0)
    {
    	mydir=opendir(curr_dir);
    	while((hell=readdir(mydir))!=NULL)
    	{
    		if(strcmp(hell->d_name,".")==0||strcmp(hell->d_name,"..")==0)
    			continue;
    		sprintf(buf,"%s/%s",curr_dir,hell->d_name);
    		stat(buf,&stat1);
    		struct passwd *pw=getpwuid(stat1.st_uid);
    		struct group *gr=getgrgid(stat1.st_gid);
    		printf((int) (S_ISDIR(stat1.st_mode)) ? "d" : "-");
    		printf((int) (stat1.st_mode & S_IRUSR) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWUSR) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXUSR) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IRGRP) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWGRP) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXGRP) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IROTH) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWOTH) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXOTH) ? "x\t" : "-\t");
    		printf("%ld\t",(long)stat1.st_nlink);
    		printf("%s\t",pw->pw_name);
    		printf("%s\t",gr->gr_name);
    		printf("%zu\t",stat1.st_size);
    		char *c=ctime(&stat1.st_mtime);
    		printf("%zu\t",stat1.st_size);
    		char ctm[20000];
    		int flg=0,j=0,cnt=0,i=0;
    		while(c[i]!='\n')
    		{
    			if(c[i]==' ')
    				flg=1;
    			if(flg)
    			{
    				if(c[i]==':')
    				{
    					cnt++;
    				}
    				if(cnt!=2)
    				{
    					ctm[j]=c[i];
    					j++;
    				}
    			}
    				if(cnt==2)
    					break;	
    				i++;
    		}
    			printf("%s\t",ctm);
    			printf("%s\n", hell->d_name);

    	}
    	return;
    }
    if(strcmp(argv[1],"-al")==0)
    {
    	mydir=opendir(curr_dir);
    	while((hell=readdir(mydir))!=NULL)
    	{
    		sprintf(buf,"%s/%s",curr_dir,hell->d_name);
    		stat(buf,&stat1);
    		struct passwd *pw=getpwuid(stat1.st_uid);
    		struct group *gr=getgrgid(stat1.st_gid);
    		printf((int) (S_ISDIR(stat1.st_mode)) ? "d" : "-");
    		printf((int) (stat1.st_mode & S_IRUSR) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWUSR) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXUSR) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IRGRP) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWGRP) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXGRP) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IROTH) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWOTH) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXOTH) ? "x\t" : "-\t");
    		printf("%ld\t",(long)stat1.st_nlink);
    		printf("%s\t",pw->pw_name);
    		printf("%s\t",gr->gr_name);
    		printf("%zu\t",stat1.st_size);
    		char *c=ctime(&stat1.st_mtime);
    		printf("%zu\t",stat1.st_size);
    		char ctm[20000];
    		int flg=0,j=0,cnt=0,i=0;
    		while(c[i]!='\n')
    		{
    			if(c[i]==' ')
    				flg=1;
    			if(flg)
    			{
    				if(c[i]==':')
    				{
    					cnt++;
    				}
    				if(cnt!=2)
    				{
    					ctm[j]=c[i];
    					j++;
    				}
    			}
    				if(cnt==2)
    					break;	
    				i++;
    		}
    			printf("%s\t",ctm);
    			printf("%s\n", hell->d_name);

    	}
    	return;
    }
    if(strcmp(argv[1],"-la")==0)
    {
    	mydir=opendir(curr_dir);
    	while((hell=readdir(mydir))!=NULL)
    	{
    		sprintf(buf,"%s/%s",curr_dir,hell->d_name);
    		stat(buf,&stat1);
    		struct passwd *pw=getpwuid(stat1.st_uid);
    		struct group *gr=getgrgid(stat1.st_gid);
    		printf((int) (S_ISDIR(stat1.st_mode)) ? "d" : "-");
    		printf((int) (stat1.st_mode & S_IRUSR) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWUSR) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXUSR) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IRGRP) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWGRP) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXGRP) ? "x" : "-");
    		printf((int) (stat1.st_mode & S_IROTH) ? "r" : "-");
    		printf((int) (stat1.st_mode & S_IWOTH) ? "w" : "-");
    		printf((int) (stat1.st_mode & S_IXOTH) ? "x\t" : "-\t");
    		printf("%ld\t",(long)stat1.st_nlink);
    		printf("%s\t",pw->pw_name);
    		printf("%s\t",gr->gr_name);
    		printf("%zu\t",stat1.st_size);
    		char *c=ctime(&stat1.st_mtime);
    		char ctm[20000];
    		int flg=0,j=0,cnt=0,i=0;
    		while(c[i]!='\n')
    		{
    			if(c[i]==' ')
    				flg=1;
    			if(flg)
    			{
    				if(c[i]==':')
    				{
    					cnt++;
    				}
    				if(cnt!=2)
    				{
    					ctm[j]=c[i];
    					j++;
    				}
    			}
    				if(cnt==2)
    					break;	
    				i++;
    		}
    			printf("%s\t",ctm);
    			printf("%s\n", hell->d_name);

    	}
    	return;
    }
    char path[1000];
    if(argv[1][0]=='/')
    	strcat(path,argv[1]);
    else
    {
    	strcpy(path,curr_dir);
    	strcat(path,"/");
    	strcat(path,argv[1]);
	}
    mydir=opendir(path);
    if(mydir==NULL)
    {
    	printf("error:invalid directory\n");
    	return;
    }
    while((hell=readdir(mydir))!=NULL)
    {
    	if(strcmp(hell->d_name,".")!=0&&strcmp(hell->d_name,"..")!=0)
    		printf("%s\n",hell->d_name);
    }	
    return;
}
void pinfo_general()
{
	int pid;
	pid = getpid();
	char buf[1000];
	char pathexe[1000];
	int temp=pid;
	sprintf(buf,"%d",temp);
	char path[]="/proc/";
	strcat(path,buf);
	strcpy(pathexe,path);
	strcat(pathexe,"/exe");
	strcat(path,"/stat");
	FILE *fptr;
	fptr = fopen(path,"r");//"r" specifies that we open the file for reading
	if (fptr == NULL)
	{
		printf("Error:Cannot open file in function pinfo_general\n");
		return;
	}
	char ch;
	char id[1000],status[10],mem[1000];
	int cnt=1,j=0;
	ch = fgetc(fptr);
	printf("pid: ");
	while (ch != EOF)
	{
		if(ch==' ')
		{
			cnt++;
			if(cnt==1)
				printf("\n");
			else if(cnt==3)
				printf("\nProcess Status: ");
			else if(cnt==24)
				printf("\nMemory: ");
			j=0;
		}
		else if(cnt==1)
		{
			printf("%c",ch);
		}
		else if(cnt==3)
		{
			printf("%c",ch);
		}
		else if(cnt==24)
		{
			printf("%c",ch);
		}
		ch = fgetc(fptr);
	}
	fclose(fptr);
	char bufexe[1000];
	int ran1=readlink(pathexe,bufexe,sizeof(bufexe));
	if(ran1==-1)
		printf("\nCannot access the executable path in function pinfo_general\n");
	else
		printf("\nExecutable Path: %s\n",bufexe);
}
void pinfo_pid(char* pid)
{
	char buf[1000];
	strcpy(buf,pid);
	char pathexe[1000];
	char path[]="/proc/";
	strcat(path,buf);
	strcpy(pathexe,path);
	strcat(pathexe,"/exe");
	strcat(path,"/stat");
	FILE *fptr;
	fptr = fopen(path,"r");//"r" specifies that we open the file for reading
	if (fptr == NULL)
	{
		printf("Error:Cannot open file in function pinfo_pid\n");
		return;
	}
	char ch;
	char id[1000],status[10],mem[1000];
	int cnt=1,j=0;
	ch = fgetc(fptr);
	printf("pid: ");
	while (ch != EOF)
	{
		if(ch==' ')
		{
			cnt++;
			if(cnt==1)
				printf("\n");
			else if(cnt==3)
				printf("\nProcess Status: ");
			else if(cnt==24)
				printf("\nMemory: ");
			j=0;
		}
		else if(cnt==1)
		{
			printf("%c",ch);
		}
		else if(cnt==3)
		{
			printf("%c",ch);
		}
		else if(cnt==24)
		{
			printf("%c",ch);
		}
		ch = fgetc(fptr);
	}
	fclose(fptr);
	char bufexe[1000];
	int ran1=readlink(pathexe,bufexe,sizeof(bufexe));
	if(ran1==-1)
		printf("\nCannot access the executable path in function pinfo_pid\n");
	else
		printf("\nExecutable Path: %s\n",bufexe);
}
int runfg(char** argv)
{
	int cnt=0;
	while(argv[cnt]!=NULL)
		cnt++;
	argv[cnt]=NULL;
	int p=fork();
	if(p<0)
	{
		printf("error:child process not created\n");
	}
	else if(p==0)
	{
		execvp(argv[0],argv);
		exit(0);
	}
	else
	{
		int status;
		globalid=p;
//		printf("%d\n",p);
//		int pin=getpid();
//		printf("%d\n",pin);
		signal(SIGINT,controlc);
		signal(SIGTSTP,controlz);
		waitpid(p,&status,0);
	}
}
/*int bg(char** argv)
{
	int pid=fork();
	if(pid<0)
	{
		printf("error:fork error\n");
	}
	else if(pid==0)
	{
		int cnt=0;
		while(argv[cnt]!=NULL)
			cnt++;
		argv[cnt]=NULL;
		execvp(argv[0],argv);		
	}
	else
	{

	}
}*/
int runbg(char** argv)
{
	int p1=fork();
	if(p1<0)
	{
		printf("error:fork unsuccessful in background process function\n");
	}
	else if(p1>0)
	{
		int pin=p1+1;
		backpid[cntback]=pin;
		strcpy(backproc[cntback++],argv[0]);
		return 1;
//		cntback++;
	}
	else if(p1==0)
	{
		setpgid(0,0);
		int p2=fork();
		if(p2<0)
		{
			printf("error:fork unsuccessful in background process function\n");
		}
		else if(p2==0)
		{
			int cnt=0;
			while(argv[cnt]!=NULL)
				cnt++;
			argv[cnt]=NULL;
			execvp(argv[0],argv);
		}
		else
		{
			printf("process ' %s ' ",argv[0]);
			printf("with pid %d has been initiated successfully\n",p2);
			int status,cid;
			cid=waitpid(p2,&status,0);
			if(WIFEXITED(status)==1)
			{
				int exitstatus=WEXITSTATUS(status);
				printf("process ' %s ' with pid %d exited normally with exit status:%d\n",argv[0],cid,exitstatus);
			}
			else
			{
				printf("process ' %s ' with pid %d exited abnormally\n",argv[0],cid); 
			}
		}
		exit(0);
	}
}
void abs_to_rel()
{
	char username[1000];
	char systemname[1000];
	getlogin_r(username,sizeof(username));
	gethostname(systemname,sizeof(systemname));
	printf("%s@%s:",username,systemname);
	char curr_dir[1000];
	getcwd(curr_dir,sizeof(curr_dir));
	char tmp[1000];
	strcpy(tmp,shell_dir);
	int l1=strlen(tmp);
	int l2=strlen(curr_dir);
	if(l1>l2)
	{
		printf("%s$",curr_dir);
		printf(" ");
		return;
	}
	else if(l1==l2)
	{
		if(strcmp(curr_dir,tmp)==0)
			printf("~");
		else
			printf("%s\n",curr_dir);
		printf("$ ");
		return;
	}
	int i=0;
	while(tmp[i]!='\0')
		i++;
	printf("~");
	while(curr_dir[i]!='\0')
	{
		printf("%c",curr_dir[i]);
		i++;
	}
	printf("$ ");
}
char** split_line(char* cmdline)
{
	int i=0;
	char **buf=malloc(100*sizeof(char*));
	for(int z=0;z<100;z++)
		buf[i]=(char*)malloc(1000*sizeof(char));
	if(buf==NULL)
	{
		printf("Memory allocation failed in split_command\n");
	}
	char* token=strtok(cmdline,";\n");
	while(token!=NULL)
	{
		buf[i++]=token;
		token=strtok(NULL,";\n");
	}
	buf[i]=NULL;
	return buf;

}
char** split_attributes(char* cmd)
{
	int i=0;
	char** buf=malloc(100*sizeof(char*));
	for(int z=0;z<100;z++)
		buf[i]=(char*)malloc(1000*sizeof(char));
	if(buf==NULL)
	{
		printf("Memory allocation failed in split_attributes\n");
	}
	char* token=strtok(cmd," ");
	while(token!=NULL)
	{
		buf[i++]=token;
		token=strtok(NULL," ");
	}
	buf[i]=NULL;
	return buf;	
}
/*int check_redirection(char** argv)
{
	int i=0,inpred=-1,outred=-1,outappend=-1;
	while(argv[i]!=NULL)
	{
		if(strcmp(argv[i],"<")==0)
			inpred=i;
		else if(strcmp(argv[i],"<")==0)
			outred=i;
		else if(strcmp(argv[i],"<")==0)
			outappend=i;
		i++;
	}
	if(inpred==0&&outred==0&&outappend==0)
	{
		return 0;
	}

}*/
/*void output_redirect(char** argv)
{
	int pid=fork();
	if(pid<0)
		printf("error:in fork in output_redirect\n");
	else if(pid>0)
	{
		return;
	}
	else
	{
		int fd=open("f.txt",O_RDWR|O_CREAT,0600);
		lseek(fd,0,SEEK_END);
//		write(copy_desc,"This will be output to the file named dup.txt\n", 46); 
          
//   write(fd,"This will also be output to the file named dup.txt\n", 51); 
		dup2(fd,1);
//		printf("yo hi!");
		execvp(argv[1],argv);
		close(fd);
		exit(1);
	}
}*/
void ioredirect(char** argv)
{
	int i=0,inpred=-1,outred=-1,outappend=-1,cnt=0,ft,stdinfd,xpos,stdoutfd;
	char infile[1000],outfile[1000];
	while(argv[i]!=NULL)
	{
		if(strcmp(argv[i],"<")==0)
			inpred=i;
		else if(strcmp(argv[i],">")==0)
			outred=i;
		else if(strcmp(argv[i],">>")==0)
			outappend=i;
		i++;
	}
//	printf("inpred:%d\n",inpred);
//	printf("outred:%d\n",outred);
//	printf("outappend:%d\n",outappend);
	if(inpred==-1&&outred==-1&&outappend==-1)
	{
		return;
	}
	if(inpred>=0)
	{
		i=0;
		while(argv[i]!=NULL)
		{
			if(!strcmp(argv[i],"<"))
			{
				if(cnt==0)
					xpos=i,cnt++;
				strcpy(infile,argv[i+1]);
			}
			i++;
		}
	}
//	printf("infile:%s\n",infile);
	if(outred>=0||outappend>=0)
	{
		i=0;
		while(argv[i]!=NULL)
		{
			if(!strcmp(argv[i],">")||!(strcmp(argv[i],">>")))
			{
				if(cnt==0)
					xpos=i,cnt++;
				strcpy(outfile,argv[i+1]);
			}
			i++;
		}
	}
	argv[xpos]=NULL;
//	close(0);
//	close(1);
	
//	printf("stdinfd:%d\n",stdinfd);
//	printf("stdoutfd:%d\n",stdoutfd);
	int pid=fork();
	if(pid<0)
	{
		perror("fork error:");
		return;
	}
	else if(pid>0)
	{
		wait(NULL);
	}
	else
	{
		int finp;
		if(inpred>=0)
		{
			struct stat check;
			if(stat(infile,&check)==-1)
			{
				printf("error:no such file\n");
				return;
			}
			int finp=open(infile,O_RDONLY,0644);
			dup2(finp,0);
		}
		int fout;
		if(outred>=0||outappend>=0)
		{
			if(outred>=0)
				fout=open(outfile,O_WRONLY|O_CREAT|O_TRUNC,0644);
			else
				fout=open(outfile,O_WRONLY|O_CREAT|O_APPEND,0644);
			dup2(fout,1);
		}
		execvp(argv[0],argv);
		close(finp);
		close(fout);
		exit(1);
	}
}
void piping(char** argv)
{
	int i=0,done=0,fd[2],j=0,k=0,p=0;
	int cnt=0,inputfd,outputfd,fread,fwrite;
//	char* comd[1000],inputf[1000],outputf[1000]; 
//	char **comd=malloc(1000*sizeof(char*));
//	char **inputf=malloc(1000*sizeof(char*));
//	char **outputf=malloc(1000*sizeof(char*));
//	char comd[100][1000];
	
	char inputf[1000],outputf[1000];
	while(1)
	{
		char* comd[100];
		for(int z=0;z<100;z++)
		comd[z]=(char*)malloc(1000*sizeof(char));
		if(argv[i]==NULL)
			return;
		if(done)
			break;
		cnt++;
		p=0;
		while(1)
		{
//			printf("%s\n",argv[j]);
			if(argv[j]==NULL)
			{
				done=1;break;
			}
			if(strcmp(argv[j],"|")==0)
				break;
			else
			{
				strcpy(comd[p],argv[j]);
/*				int r;
				int l=strlen(argv[j]);
				for(r=0;r<l;r++)
					comd[p][r]=argv[j][r];
				comd[p][r]='\0';*/
//				strcpy(comd[p],argv[j]);
				p++;
			}
			j++;
		}
		j++;
		comd[p]=NULL;
		if(cnt==1)
		{
			int fl=pipe(fd);
			if(fl==-1)
			{
				printf("error:error in piping\n");
				return;
			}
			k=0;
			while(comd[k]!=NULL)
			{
				if(!strcmp(comd[k],"<"))
				{
					comd[k]=NULL;
					strcpy(inputf,comd[k+1]);
					inputfd=dup(0);
					fread=open(inputf,O_RDONLY,0644);
					dup2(fread,0);
					close(fread);
				}
				k++;
			}
			outputfd=dup(1);
			dup2(fd[1],1);
			close(fd[1]);
		}
		else if(done==1)
		{
			k=0;
			while(comd[k]!=NULL)
			{
				if(!strcmp(comd[k],">>"))
				{
					comd[k]=NULL;
					strcpy(outputf,comd[k+1]);
					outputfd=dup(1);
					fwrite=open(outputf,O_WRONLY|O_CREAT|O_APPEND,0644);
					dup2(fwrite,1);
					close(fwrite);
				}
				else if(!strcmp(comd[k],">"))
				{
					comd[k]=NULL;
					strcpy(outputf,comd[k+1]);
					outputfd=dup(1);
					fwrite=open(outputf,O_WRONLY|O_CREAT|O_TRUNC,0644);
					dup2(fwrite,1);
					close(fwrite);
				} 
				k++;
			}
			inputfd=dup(0);
			dup2(fd[0],0);
			close(fd[0]);
		}
		else
		{
			inputfd=dup(0);
			dup2(fd[0],0);
			pipe(fd);
			outputfd=dup(1);
			dup2(fd[1],1);
			close(fd[1]);
		}
		int pid=fork();
		if(pid<0)
		{
			perror("error in fork:");
			return;
		}
		else if(pid>0)
		{
			wait(NULL);
			dup2(inputfd,0);
			dup2(outputfd,1);
		}
		else
		{
			execvp(comd[0],comd);
		}
		for(int z=0;z<100;z++)
		free(comd[z]);
//		free(comd);
	}
}
void jobs()
{
	int res=0,z;
	char buf[1000];
//	printf("%d\n",cntback);
	for(z=0;z<cntback;z++)
	{
		int pin=backpid[z];
		if(bgflag[z]==1)
		{
			continue;					
		}
		res++;
		sprintf(buf,"%d",pin);
//		strcpy(buf,pid);
		char pathexe[1000];
		char path[]="/proc/";
		strcat(path,buf);
		strcat(path,"/stat");
//		printf("path:%s\n",path);
		FILE *fptr;
		fptr = fopen(path,"r");//"r" specifies that we open the file for reading
		if (fptr == NULL)
		{
			continue;
		}
//		printf("%s\n", );
		char ch;
		char id[1000],status[10],mem[1000];
		int cnt=1,j=0;
		ch = fgetc(fptr);
//		printf("yo1\n");
//		printf("pid: ");
		while (ch != EOF)
		{
//			if(cnt==3)
//			printf("ch:%c\n",ch);
			if(ch==' ')
			{
				cnt++;
//				if(cnt==3)
//					printf("\nProcess Status: ");
				j=0;
			}
			else if(cnt==3)
			{
				if(ch=='R'||ch=='S')
				{
					printf("[%d] Running %s [%d]\n",res,backproc[z],pin);
				}
				else if(ch=='T')
				{
					printf("[%d] Stopped %s [%d]\n",res,backproc[z],pin);
				}
//				printf("%c",ch);
			}
			ch = fgetc(fptr);
		}
		fclose(fptr);
	}
}
void checkjob()
{
	int res=0,z;
	char buf[1000];
//	printf("%d\n",cntback);
	for(z=0;z<cntback;z++)
	{
		int pin=backpid[z];
//		printf("backpid[%d]:%d\n",z,backpid[z]);
//		printf("backflag[%d]:%d\n",z,bgflag[z]);
		if(bgflag[z]==1)
		{
			continue;					
		}
		res++;
		sprintf(buf,"%d",pin);
//		strcpy(buf,pid);
		char pathexe[1000];
		char path[]="/proc/";
		strcat(path,buf);
		strcat(path,"/stat");
//		printf("path:%s\n",path);
		FILE *fptr;
		fptr = fopen(path,"r");//"r" specifies that we open the file for reading
		if (fptr == NULL)
		{
			bgflag[z]=1;
			continue;
		}
//		printf("yo2\n");
		char ch;
		char id[1000],status[10],mem[1000];
		int cnt=1,j=0;
		ch = fgetc(fptr);
//		printf("pid: ");
		while (ch != EOF)
		{
			if(ch==' ')
			{
				cnt++;
				j=0;
			}
			else if(cnt==3)
			{
				if(ch=='Z')
				bgflag[z]=1;
			}
			ch = fgetc(fptr);
		}
		fclose(fptr);
	}
}
void kjob(char** argv)
{
	if(argv[1]==NULL||argv[2]==NULL)
	{
		printf("error:invalid number of arguments\n");
		return;
	}
	int signal=atoi(argv[2]);
	int job=atoi(argv[1]);
	int z=0,fl=0,res=0,pin;
	for(z=0;z<cntback;z++)
	{
		if(bgflag[z]==0)
		{
			res++;
			if(res==job)
			{
				fl=1;
				pin=backpid[z];
				break;
			}
		}
	}
	if(fl==1)
	{
		kill(pin,signal);
	}
	else
	{
		printf("error:invalid job number\n");
	}
}
void bg(char** argv)
{
	if(argv[1]==NULL)
	{
		printf("error:invalid arguments\n");
		return;
	}
	int job=atoi(argv[1]);
	int z=0,fl=0,res=0,pin;
	for(z=0;z<cntback;z++)
	{
		if(bgflag[z]==0)
		{
			res++;
			if(res==job)
			{
				fl=1;
				pin=backpid[z];
				break;
			}
		}
	}
	if(fl==1)
	{
		if(kill(pin,SIGCONT)<0)
		{
			printf("error:job doesnt exist\n");
			return;
		}
	}
	else
	{
		printf("error:invalid job number\n");
	}
}
void fg(char** argv)
{
	if(argv[1]==NULL)
	{
		printf("error:invalid arguments\n");
		return;
	}
	int job=atoi(argv[1]);
	int z=0,fl=0,res=0,pin;
	for(z=0;z<cntback;z++)
	{
		if(bgflag[z]==0)
		{
			res++;
			if(res==job)
			{
				fl=1;
				pin=backpid[z];
				break;
			}
		}
	}
	if(fl==0)
		return;
//	printf("yo2\n");
	kill(pin,SIGCONT);
//	printf("yo3\n");
//	printf("pid:%d\n",pin);
	pin--;
	int cid,status;
	globalid=pin+1;
	signal(SIGINT,controlc);
	signal(SIGTSTP,controlz);
	cid= waitpid(pin, &status, WUNTRACED);
/*	int status,cid;
	while(1)
	{
		if(WIFEXITED(status) && WIFSIGNALED(status))
		{
			cid= waitpid(pin, &status, WUNTRACED);
			continue;
		}
		break;
	}*/
//	int cid=waitpid(pin,&status,WUNTRACED);
/*	if(WIFEXITED(status)==1)
	{
		int exitstatus=WEXITSTATUS(status);
		printf("process ' %s ' with pid %d exited normally with exit status:%d\n",argv[0],cid,exitstatus);
	}
	else
	{
		printf("process ' %s ' with pid %d exited abnormally\n",argv[0],cid); 
	}*/
//		printf("error:job doesnt exist\n");
//		return;
//	}
//	else
//	{
//		printf("error:invalid job number\n");
//	}
/*	int wpid,status;
	while(!WIFEXITED(status) && !WIFSIGNALED(status))
	{
		printf("yo1\n");
		wpid=waitpid(pin,&status,WUNTRACED);
	}*/
}
void overkill()
{
	int z=0,res=0;
	for(z=0;z<cntback;z++)
	{
		if(bgflag[z]==0)
		{
			kill(backpid[z],SIGINT);
			bgflag[z]=1;
		}
	}
}
void cronjob(char** argv)
{
	int l=0,r=0;
	char* comd[10];
	for(int z=0;z<10;z++)
		comd[z]=(char*)malloc(100*sizeof(char));
	int i=0;
	while(strcmp(argv[l],"-c")!=0)
		l++;
	i=0;
	while(strcmp(argv[r],"-t")!=0)
		r++;
	l++;
	r++;
//	printf("%d\n",l);
//	printf("%d\n",r);
//	printf("yo1\n");
	int p=0;i=l;
	while(i!=r-1)
	{
		strcpy(comd[p],argv[i]);
		p++;
		i++;
	}
//	printf("yo2\n");
	comd[p]=NULL;
	i=0;
	while(comd[i]!=NULL)
	{
		printf("%s\n",comd[i]);
		i++;
	}
	int interval=atoi(argv[r]);
	int total=atoi(argv[r+2]);
//	printf("total:%d\n",total);
//	printf("interval:%d\n",interval);
	while(total>0)
	{
		int pid=fork();
		if(pid==0)
		{
//		while(total>0)
//		{
			sleep(interval);
			execvp(comd[0],comd);
//		}
		}
		else if(pid>0) 
		{
			wait(NULL);
			total-=interval;
//			printf("%d\n",total);
		}
	}
}
void setenvfunction(char **args)
{
	if(args[0]==NULL || args[3]!=NULL)
 		{printf("Invalid arguments\n");return;}
	if (args[2] == NULL)
	{
		args[2] = " ";
	}
	if (setenv(args[1],args[2],1) != 0) 
	   printf("error:setenv error\n");
}
void unsetenvfunction(char **args)
{
	if(args[0]==NULL || args[3]!=NULL||args[1]==NULL) 
		 {printf("Invalid arguments\n");return;}
	if (unsetenv(args[1]) != 0) 
		printf("error:unsetenv error\n");
	return;
}
void getenvfunction(char **args)
{
	if(args[1]==NULL)
	{
		printf("invalid arguments\n");
		return;
	}
	if(getenv(args[1])==NULL)
	{
		printf("Not available\n");
		return;
	}
	else
	{
		printf("%s\n",getenv(args[1]));
	}
}
void controlc(int signum)
{
//	int pid=getpid();

//	printf("pid:%d\n",globalid);
	if(globalid>0)
	kill(globalid,signum);
}
void controlz(int signum)
{
	if(globalid>0)
	kill(globalid,signum);
}
/*void input_redirect(char** argv)
{
	int pid=fork();
	if(pid<0)
		printf("error:in fork in output_redirect\n");
	else if(pid>0)
	{
		return;
	}
	else
	{
		int fd=open("f.txt",O_RDWR|O_CREAT,0600);
//		lseek(fd,0,SEEK_END);
//		write(copy_desc,"This will be output to the file named dup.txt\n", 46); 
          
//   write(fd,"This will also be output to the file named dup.txt\n", 51); 
		dup2(fd,0);
//		printf("yo hi!");
		execvp(argv[1],argv);
		close(fd);
		exit(1);
	}
}*/
/*void pipe(char* cmd)
{
	int i=0;
    for( i=1;i<argc-1;i++)
    {
        int pd[2];
        pipe(pd);
        int pid=fork();
        if (pid==0)
        {
            dup2(pd[1],1); 
            execlp(argv[i], argv[i], NULL);
            perror("exec");
            abort();
        }
        dup2(pd[0], 0);
        close(pd[1]);
    }

    execlp(argv[i], argv[i], NULL);
    perror("exec");
    abort();
}*/
int execute_command(char ** argv)
{
	int input=0,output=0,output_append=0,pipe=0;
	int i=0;
//	printf("yo1\n");
	while(argv[i]!=NULL)
	{
		if(!strcmp(argv[i],"<"))
			input=1;
		else if(!strcmp(argv[i],">"))
			output=1;
		else if(!strcmp(argv[i],">>"))
			output_append=1;
		else if(!strcmp(argv[i],"|"))
			pipe=1;
		i++;
	}
	if(pipe>0)
	{
//		printf("yo2\n");
		piping(argv);
		return 1;
	}
	if(pipe==0&&(input==1||output==1||output_append==1))
	{
		ioredirect(argv);
		return 1;
	}
	if(strcmp(argv[0],"jobs")==0)
	{
		jobs();
		return 1;
	}
	if(strcmp(argv[0],"kjob")==0)
	{
		kjob(argv);
		return 1;
	}
	if(strcmp(argv[0],"bg")==0)
	{
		bg(argv);
		return 1;
	}
	if(strcmp(argv[0],"fg")==0)
	{
		fg(argv);
		return 1;
	}
	if(strcmp(argv[0],"overkill")==0)
	{
		overkill();
		return 1;
	}
	if(strcmp(argv[0],"setenv")==0)
	{
		setenvfunction(argv);
		return 1;
	}
	if(strcmp(argv[0],"unsetenv")==0)
	{
		unsetenvfunction(argv);
		return 1;
	}
	if(strcmp(argv[0],"getenv")==0)
	{
		getenvfunction(argv);
		return 1;
	}
	if(strcmp(argv[0],"cronjob")==0)
	{
		cronjob(argv);
		return 1;
	}
	if(strcmp(argv[0],"ls")==0)
	{
		ls(argv);
		return 1;
	}
	if(strcmp(argv[0],"cd")==0)
	{
		cd(argv[1]);
		return 1;
	}
	if(strcmp(argv[0],"pwd")==0)
	{
		pwd(argv);
		return 1;
	}
	if(strcmp(argv[0],"pinfo")==0)
	{

		if(argv[1]==NULL||strcmp(argv[1],"&")==0)
		{
//			printf("sfsj\n");
			pinfo_general();
		}
		else
			pinfo_pid(argv[1]);
		return 1;
	}
	if(strcmp(argv[0],"echo")==0)
	{
		echo(argv);
		return 1;
	}
	int cnt=0;
	while(argv[cnt]!=NULL)
		cnt++;
	cnt--;
	if(strcmp(argv[cnt],"&")==0)
	{
		runbg(argv);
		return 1;
	}
	runfg(argv);
	return 1;
}
int main(int cntarg)
{
	getcwd(shell_dir,sizeof(shell_dir));
	signal(SIGINT,controlc);
	signal(SIGTSTP,controlz);
//	globalid=getpid();
	while(1)
	{
		abs_to_rel();
		char* line=read_line();
		char** cmdline=split_line(line);
		for(int i=0;cmdline[i]!=NULL;i++)
		{
//			add_history(cmdline[i]);
			checkjob();
			if(strcmp(cmdline[i],"quit")==0)
				exit(0);
			char** argv=split_attributes(cmdline[i]);
/*			i=0;
			while(argv[i]!=NULL)
			{
				printf("%s\n",argv[i]);
				i++;
			}*/
			execute_command(argv);
//			ioredirect(argv);
//			sum();
		}
	}
	return 0;
}