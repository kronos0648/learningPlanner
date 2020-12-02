#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct account {
	char name[100];
	char id[100];
	char password[100];
	struct account* link;
}Account;

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
	printf("이름 입력:");
	fgets(instAccount->name, sizeof(instAccount->name), stdin);
	printf("ID 입력:");
	fgets(instAccount->id, sizeof(instAccount->id), stdin);
	printf("PASSWORD 입력:");
	fgets(instAccount->password, sizeof(instAccount->password), stdin);

	temp->link = instAccount;

	fwrite(instAccount, sizeof(Account), 1, fp);

	fclose(fp);
}

void search_record(Account* ap) { //검색
	char id[100];
	char password[100];
	printf("ID 입력: ");
	getchar();
	fgets(id, sizeof(id), stdin);
	printf("PASSWORD 입력: ");
	fgets(password, sizeof(password), stdin);
	//name[strlen(name) - 1] = '\0';
	Account* temp = ap->link;
	while (temp != NULL) {
		if (strcmp(temp->id, id) == 0 && strcmp(temp->password, password) == 0) {
			printf("---------------------------------\n");
			printf("이름 : %s", temp->name);
			printf("ID : %s", temp->id);
			printf("PASSWORD : %s", temp->password);
			printf("---------------------------------\n");
			break;
		}
		temp = temp->link;
	}
}


void allAccount(Account* ap) { //전부 출력
	Account* temp = ap->link;
	while (temp != NULL) {
		printf("---------------------------------\n");
		printf("이름 : %s", temp->name);
		printf("ID : %s", temp->id);
		printf("PASSWORD : %s", temp->password);
		temp = temp->link;
	}
	printf("---------------------------------\n");
}


void main() {
	int num = 0;

	Account* ap = (Account*)malloc(sizeof(Account));
	irc(ap);

	FILE* fp;
	fp = fopen("user.txt", "rb");

	if (fp != NULL) {
		fileLoad(fp, ap);
		fclose(fp);
	}

	while (1) {
		printf("------------------------------\n");
		printf("학사일정 관리 플래너\n");
		printf("1. 회원가입\n");
		printf("2. 로그인\n");
		printf("3. 계정확인(임시,개발후 삭제)\n");
		printf("4. 종료\n");
		printf("------------------------------\n");

		scanf("%d", &num);
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
			allAccount(ap);
			break;
		default:
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