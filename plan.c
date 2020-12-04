#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

typedef struct schd
{
	char id[100];
	char date[100];
	char subject[100];
	char plan[100];
	struct schd *link;
}Schedule;



Schedule getSchedule(char *id)
{
	int fd=open("schedule",O_RDONLY);
	if(fd==-1)
	{
		perror("Can't open file");
	}
	off_t sz_file=lseek(fd,0,SEEK_END);
	lseek(fd,0,SEEK_SET);
	long position;
	struct flock lock;
	lock.l_type=F_RDLCK;
	lock.l_whence=0;
	lock.l_start=position;
	lock.l_len=sizeof(Schedule);
	if(fcntl(fd,F_SETLKW,&lock)==-1)
	{
		perror("lock error\n");
	}
	
	char schedule[sz_file];
	int result;
	if((result=read(fd,schedule,sz_file))==-1)
	{
		perror("read error");
	}
	int tokenCount=0;
	char *ptr=strtok(schedule,"/");
	char info[200][200];
	while(ptr!=NULL)
	{
		strcpy(info[tokenCount],ptr);
		ptr=strtok(NULL,"/");
		tokenCount++;
	}
	for(int i=0;i<tokenCount;i+=4)
	{
		if(strcmp(info[i],id)==0)
		{
			printf("===================================\n");
			printf("Date : %s\n",info[i+1]);
			printf("Subject : %s\n",info[i+2]);
			printf("Plan : %s\n",info[i+3]);
		}	
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	
}

void registerSchedule(char *id)
{
	int fd_schedule;
	if((fd_schedule=open("schedule",O_CREAT|O_WRONLY|O_APPEND,0777))==-1)
	{
		perror("File open error");
	}
	off_t sz_file=lseek(fd_schedule,0,SEEK_END);
	lseek(fd_schedule,0,SEEK_SET);
	off_t position=sz_file;
	struct flock lock;
	lock.l_type=F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=position;
	if(fcntl(fd_schedule,F_SETLKW,&lock)==-1)
	{
		perror("File is lock");
		//return;
	}

	char subject[100];
	char date[100];
	char plan[100];
	printf("write the subject to register schedule as\n");
	printf("---------------------------------\n");
	scanf("%s",subject);
	getchar();
	printf("write the date for plan(Format : yyyy-mm-dd)\n");
	printf("---------------------------------\n");
	scanf("%s",date);
	getchar();
	printf("write the schedule as\n");
	printf("---------------------------------\n");
	scanf("%s",plan);
	getchar();
	int size=sizeof(id)+sizeof(date)+sizeof(subject)+sizeof(plan)+10;
	char *insert=(char*)malloc(size);

	strcpy(insert,id);
	strcat(insert,"/");
	strcat(insert,date);
	strcat(insert,"/");
	strcat(insert,subject);
	strcat(insert,"/");
	strcat(insert,plan);
	strcat(insert,"\0");
	strcat(insert,"/");
	if(write(fd_schedule,insert,strlen(insert))==-1)
	{
		perror("Write Error");
	}
	lock.l_type=F_UNLCK;
	fcntl(fd_schedule,F_SETLK,&lock);
	close(fd_schedule);
}
