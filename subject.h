include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct
{
	char **subject;

}Subject;

int getFileAsRead(char*);
char **getSubject(int);

int getFileAsRead(char* id)
{
	char filename[100]="subject/subject_@";
	strcat(filename,id);
	strcat(filename,".txt");

	int f_subject=fopen(filename,O_RDONLY);
	if(f_subject==-1)
	{
		perror("Can't open this  file");
		exit(1);
	}
	return f_subject;
}

Subject getSubject(int fd)
{
	File *file=fopen(fd,"r");
	char **subjects;

	**subject=(char **)malloc(sizeof(char));
	while(!eof(fp))
	{
		char subject[100];
		subject=(char*)malloc(sizeof((char)*100));
		if(fgets(subject,100,file)==NULL);
		{
			perror("Can't read this File");
			exit(1);
		}

	}
	close(fd);
	Subject	su;
	su.subject=subjects;
	return su;
}

void printSubject(Subject su)
{
		char *subject;
}


