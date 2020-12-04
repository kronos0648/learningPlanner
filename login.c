#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "subject.c"
#include "plan.c"

typedef struct account {
	char name[100];
	char id[100];
	char password[100];
	struct account* link;
}Account;

char *loggedId;

void irc(Account* p) {
	sprintf(p->name, "null");
	sprintf(p->id, "null");
	sprintf(p->password, "null");
	p->link = NULL;
}

void fileLoad(FILE* fp, Account* ap) {
	Account* temp;
	Account* last;
	temp = (Account*)malloc(sizeof(Account));
	fread(temp, sizeof(Account), 1, fp);
	if (feof(fp) == 0) {
		ap->link = temp;
		last = temp;
	}
	else free(temp);

	temp = (Account*)malloc(sizeof(Account));
	fread(temp, sizeof(Account), 1, fp);
	if (feof(fp) == 0) {
		last->link = temp;
		last = temp;
	}
	else free(temp);

	while (1) {
		temp = (Account*)malloc(sizeof(Account));
		fread(temp, sizeof(Account), 1, fp);
		if (feof(fp) == 0) {
			last->link = temp;
			last = temp;
		}
		else {
			free(temp);
			break;
		}
	}
	printf("--------------------------------\n");
}

void add_record(FILE* fp, Account* ap) {
	fp = fopen("user.txt", "a+b");
	if (fp == NULL) {
		printf("open error\n");
		return;
	}

	Account* instAccount = (Account*)malloc(sizeof(Account));
	irc(instAccount);
	Account* temp = ap;
	while (temp->link != NULL) {
		temp = temp->link;
	}
	printf("---------------------------------\n");
	getchar();
	printf("name:");
	fgets(instAccount->name, sizeof(instAccount->name), stdin);
	printf("ID:");
	fgets(instAccount->id, sizeof(instAccount->id), stdin);
	printf("PASSWORD:");
	fgets(instAccount->password, sizeof(instAccount->password), stdin);

	temp->link = instAccount;

	fwrite(instAccount, sizeof(Account), 1, fp);
	fclose(fp);
	/*char filename[100];
	strcpy(filename,"subject/subject_");
	strcat(filename,instAccount->id);
	FILE *fw;
	fw=fopen(filename,"w");
	printf("filename:%s\n",filename);
	close(fw);
	strcpy(filename,"schedule/schedule_");
	strcat(filename,instAccount->id);
	fw=fopen(filename,"w");
	printf("filename:%s\n",filename);
	close(fw);*/
}



void search_record(Account *ap)
{
	char id[100];
	char password[100];
	printf("ID: ");
	getchar();
	fgets(id, sizeof(id), stdin);
	printf("PASSWORD: ");
	fgets(password, sizeof(password), stdin);
	//name[strlen(name) - 1] = '\0';
	Account* temp = ap->link;
	while (temp != NULL) {
		//printf("ID:%s\n",temp->id);
		if (strcmp(temp->id, id) == 0 && strcmp(temp->password, password) == 0) {
			loggedId=(char*)malloc(sizeof(id));
			char *ptr=strtok(id,"\n");
			strcpy(loggedId,ptr);
			printf("---------------------------------\n");
			printf("login success\n");
			printf("---------------------------------\n");
			management();
			break;
		}
		temp = temp->link;
	}
}


void allAccount(Account* ap) { //���� ���
	Account* temp = ap->link;
	while (temp != NULL) {
		printf("---------------------------------\n");
		printf("name : %s", temp->name);
		printf("ID : %s", temp->id);
		printf("PASSWORD : %s", temp->password);
		temp = temp->link;
	}
	printf("---------------------------------\n");
}

void regsubject()
{
	char subject[100];
	printf("write the subject to register\n");
	printf("---------------------------------\n");
	scanf("%s",subject);
	printf("LoggedId:%s Subject:%s\n",loggedId,subject);
	registerSubject(loggedId,subject);
	printf("register success\n");

}

void management()
{
	int num=0;
	short run=1;
	while(run)
	{
		printf("---------------------------------\n");
		printf("1.register Schedule 2.view Schedule 3.logout 4.exit\n");
		printf("---------------------------------\n");
		scanf("%d", &num);
		getchar();
		switch(num)
		{
			case 1:
			registerSchedule(loggedId);
			break;

			case 2:
			getSchedule(loggedId);
			break;

			case 3:
			free(loggedId);
			authentication();
			break;

			case 4:
			run=0;
			break;

			default:
			printf("Error : Undefined Option\n");
			break;
		}
	}

}

void authentication()
{
	int num = 0;
	short run=1;

	Account* ap = (Account*)malloc(sizeof(Account));
	irc(ap);

	FILE* fp;
	fp = fopen("user.txt", "rb");

	if (fp != NULL) {
		fileLoad(fp, ap);
		fclose(fp);
	}

	while (run) {
		printf("------------------------------\n");
		printf("learning management planner\n");
		printf("1. Sign Up\n");
		printf("2. Login\n");
		//printf("3. Account Check\n");
		printf("3. Exit\n");
		printf("------------------------------\n");

		scanf("%d", &num);
		//getchar();
		if (num == 4)
			break;

		switch (num) {
		case 1:
			add_record(fp, ap);
			break;
		case 2:
			search_record(ap);
			break;
		case 3:
			//allAccount(ap);
			run=0;
			break;
		default:
			printf("Error : undefined Option\n");
			break;
		}

	}

	{
		Account* temp = ap;
		Account* temp2;
		while (temp != NULL)
		{
			temp2 = temp->link;
			free(temp);
			temp = temp2;
		}
	}

}

void main() {
	authentication();
}
