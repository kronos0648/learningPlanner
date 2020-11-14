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
	int column;
}Schedule;

int getFileAsRead(char*);
char **getSchedule(int);


int getFileAsRead(char* id)
{
	char filename[100]="schedule/schedule_@";
	strcat(filename,id);
	strcat(filename,".txt");
	
	int f_schedule=fopen(filename,O_RDONLY);
	if(f_schedule==-1)
	{
		perror("Can't open file");
		exit(1);
	}
	return f_schedule;
}

Schedule getSchedule(int fd)
{
	File *file=fdopen(fd,"r");
	char **schedules;
	int row=1;
	int col=0;
	
	**schedules=(char **)malloc(sizeof((char)*row));
	while(!eof(fp))
	{
		char schedule[100];
		schedule=(char*)malloc(sizeof((char)*100));
		if(fgets(schedule,100,file)==NULL)
		{
			perror("can't read File");
			exit(1);
		}
		
		if(col<sizeof(schedule))
		{
			col=sizeof(schedule);
		}
		schedules[row]=malloc(sizeof((char)*col));
		strcpy(schedules[row],schedule);
		row++;
	}
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
		char scd
	}
}
