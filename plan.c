#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct
{
	char **schedule;
	int row;
	int col;
}Schedule;

int getFileAsRead(char*);
Schedule getSchedule(int fd);
void printSchedule(Schedule);


int getFileAsRead(char* id)
{
	char filename[100]="schedule_@";
	strcat(filename,id);
	
	int f_schedule=open(filename,O_RDONLY);
	if(f_schedule==-1)
	{
		perror("Can't open file");
		exit(1);
	}
	return f_schedule;
}

Schedule getSchedule(int fd)
{
	FILE* file=fdopen(fd,"r");
	char **schedules;
	int row=1;
	int col=0;
	long position;
	struct flock lock;
	
	**schedules=(char **)malloc(sizeof((char*)row));
	while(!feof(file))
	{
		lock.l_type=F_RDLCK;
		lock.l_whence=0;
		lock.l_start=position;
		lock.l_len=sizeof(Schedule);
		if(fcntl(fd,F_SETLKW,&lock)==-1)
		{
			printf("lock error\n");
			exit(-1);
		}
		lseek(fd,position,0);
		char *schedule;
		int size=sizeof(char*)*100;
		schedule=(char*)malloc(size);
		if(fgets(schedule,100,file)==NULL)
		{
			perror("can't read File");
			lock.l_type=F_UNLCK;
			fcntl(fd,F_SETLK,&lock);
		}
		
		if(col<sizeof(schedule))
		{
			col=sizeof(schedule);
		}
		schedules[row]=malloc(sizeof((char*)col));
		strcpy(schedules[row],schedule);
		row++;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	Schedule s;
	s.schedule=schedules;
	s.row=row;
	s.col=col;
	return s;
}

void printSchedule(Schedule s)
{
	for(int i=0;i<s.row;i++)
	{
		char *date;
		char *subject;
		char *content;
		char scd[s.col];
		strcpy(scd,s.schedule[i]);
		char *ptr=strtok(scd,"/");
		char *seq[100]={"날짜:","과목:","내용:"};
		int position=0;
		while(ptr!=NULL)
		{
			strcat(seq[position],ptr);
			printf("%s ",seq[position]);
			i++;
			if(i==2) printf("\n");
		}
	}
}

void registerSchedule(char *id,char *date,char *subject,char *content)
{
	long position;
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=0;
	lock.l_start=position;
	lock.l_len=sizeof(Schedule);
	
	
	char filename[100]="schedule/schedule_@";
	strcat(filename,id);
	int fd_schedule=open(filename,O_APPEND);
	if(fcntl(fd_schedule,F_SETLKW,&lock)==-1)
	{
		printf("write lock error\n");
		return;
	}
	lseek(fd_schedule,position,0);
	FILE* f_schedule=fdopen(fd_schedule,"a");
	
	int size=sizeof(date)+sizeof(subject)+sizeof(content)+4;
	char *insert=malloc(size);
	strcmp(insert,date);
	strcat(insert,"/");
	strcat(insert,subject);
	strcat(insert,"/");
	strcat(insert,content);
	
	fputs(insert,f_schedule);
	
	lock.l_type=F_UNLCK;
	fcntl(fd_schedule,F_SETLK,&lock);
	close(f_schedule);
	close(fd_schedule);
	
}
