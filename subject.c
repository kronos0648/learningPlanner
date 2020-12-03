#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct
{
	char **subject;
	int row;
	int col;
}Subject;

int getFileAsRead(char*);
Subject getSubject(int fd);
void printSubject(Subject);


int getFileSubject(char* id)
{
	char filename[100]="subject/subject_@";
	strcat(filename,id);
	strcat(filename,".txt");

	int f_subject=open(filename,O_RDONLY);
	if(f_subject==-1)
	{
		perror("Can't open this file");
		exit(1);
	}
	return f_subject;

}

Subject getSubject(int fd)
{
	FILE* file=fdopen(fd,"r");
	char **subjects;
	int row=1;
	int col=0;
	long pos;
	struct flock lock;

	int sjt_size=sizeof(char)*row;
	subjects=(char **)malloc(sjt_size);
	while(!feof(file))
	{
		lock.l_type=F_RDLCK;
		lock.l_whence=0;
		lock.l_start=pos;
		lock.l_len=sizeof(Subject);
		if(fcntl(fd,F_SETLKW,&lock)==-1)
		{
			printf("lock error\n");
			exit(-1);
		}
		lseek(fd,pos,0);
		char *subject;
		int size=sizeof(char*)*100;
		subject=(char*)malloc(size);
		if(fgets(subject,100,file)==NULL)
		{
			perror("Cant' read this file");
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
		}
		subjects[row]=(char*)malloc(sizeof(char)*col);
		strcpy(subjects[row],subject);
		row++;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	Subject s;
	s.subject=subjects;
	s.row=row;
	s.col=col;
	return s;
}

void printSubject(Subject s)
{
	for(int i=0;i<s.row;i++)
	{
		char *subject;
		char scd[s.col];
		strcpy(scd,s.subject[i]);
		char *ptr=strtok(scd,"\n");
		char *seq[100]={"Subject : "};
		int pos=0;
		while(ptr!=NULL)
		{
			strcat(seq[pos],ptr);
			printf("%s",seq[pos]);
			i++;
			if(i==2) printf("\n\n");
		}
	}
}

void registerSubject(char *id, char *subject)
{
	printf("112num2222");
	long pos;
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=0;
	lock.l_start=pos;
	lock.l_len=sizeof(Subject);

	printf("112num");
	//char filename[100]="subject/subject_@";
	//strcat(filename,id);
	//strcat(filename,".txt");
	int fd_subject;
	fd_subject=open("subject",O_APPEND);
	printf("%d",fd_subject);
	/*if((fd_subject=open("subject",O_APPEND)==-1))
	{
		perror("file open error");
	}*/
	/*if(fcntl(fd_subject,F_SETLKW,&lock)==-1)
	{
		perror("Write lock error");
		//return;
	}*/

	lseek(fd_subject,pos,0);
	FILE* f_subject=fdopen(fd_subject,"a");

	int size=sizeof(id)+sizeof(subject)+4;
	char *insert=(char*)malloc(size);
	strcpy(insert,id);
	strcat(insert,"/");
	strcat(insert,subject);
	//strcat(insert,"\n");

	fputs(insert,f_subject);

	lock.l_type=F_UNLCK;
	fcntl(fd_subject,F_SETLK,&lock);
	//close(f_subject);
	close(fd_subject);
}
