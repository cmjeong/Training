/******************************************************************************
 *
 *  File Name : user_man.c
 *
 *  File creation : 2017-08-17
 *  Function : Administer user information database
 *
 ******************************************************************************/

#include "header.h"

#define USER_DB 	"user_db.txt"

int user_cnt;

int menu();
int read_db();
int login();
int check_id(char *input);
int check_pw(char *input, int index);
int join();
// int delete();

void user_man()
{
	int loop = 1;
	memset(user_DB, 0x00, sizeof(user_DB));
	if(read_db() == -1)
		printf("There is no user in database. Create new id.\n");

	while(loop)
	{
		switch(menu())
		{
			case 1:
				loop = login();
				break;
			case 2: 
				loop = join();
				break;
			case 3: 
				exit(1);
		}
	}
}


int menu()
{
	int m;

	printf(" --- M E N U ---\n");
	printf(" - 1. Log in	\n");
	printf(" - 2. Join	\n");
	printf(" - 3. Quit	\n");
	printf(" ---------------\n");
	printf("Input number : ");
	scanf("%d", &m);
	__fpurge(stdin);
	return m;
}

int read_db()
{
	FILE *fp;
	char buf[128];
	char *tmp;
	int cnt = 0;

	fp = fopen(USER_DB, "r");
	if(fp == NULL)
		return -1;

	while(!feof(fp))
	{
		if(!fgets(buf, 1024, fp))
			break;
		if(buf[strlen(buf)-1] == '\n')
			buf[strlen(buf)-1] = '\0';
		tmp = strtok(buf, " ");
		if(tmp)
			strcpy(user_DB[cnt].id, tmp);
		tmp = strtok(NULL, " ");
		if(tmp)
			strcpy(user_DB[cnt].pw, tmp);
		cnt++;
	}
	user_cnt = cnt;
	fclose(fp);
	return 0;
}

int login()
{
	char input_id[20];
	char input_pw[20];
	int id_index;
	while(1)
	{
		printf("ID(e : exit to menu) : ");
		FGETS(input_id, 20, stdin);
		if(!strcmp(input_id , "e"))
			return 1;
		if((id_index = check_id(input_id)) == -1)
			printf("Cannot find id. Input again.\n");
		else
		{
		
			printf("[%s] Password : ", input_id);
			FGETS(input_pw, 20, stdin);
			if(check_pw(input_pw, id_index) == -1)
				printf("Input wrong password. Input again.\n");
			else
				break;
		}
	}
	user_index = id_index;
	return 0;
}

int check_id(char *input)
{
	int i;
	for(i=0; i<user_cnt; i++)
	{
		if(strcmp(user_DB[i].id, input) == 0)
			return i;
	}
	return -1;
}

int check_pw(char *input, int index)
{
	if(strcmp(user_DB[index].pw, input) == 0)
		return 0;
	else
		return -1;
}

int join()
{
	char new_id[40];
	char new_pw[40];
	printf("Welcome! Create New ID for join us.\n");
	while(1)
	{
		printf("Input ID & Password (5~20), input 'e' to exit menu.\n");
		printf("New ID : ");
		FGETS(new_id, 40, stdin);
		if(!strcmp(new_id, "e"))
			return 1;
		if(strlen(new_id) < 5 || strlen(new_id) > 20)
			printf("Input 5~20 characters for id.\n");
		else if(check_id(new_id) == 0)
			printf("[%s] is exist. Input another id.\n", new_id);
		else
			break;
	}
	while(1)
	{
		printf("PW : ");
		FGETS(new_pw, 40, stdin);
		if(strlen(new_pw) < 5 || strlen(new_pw) > 20)
			printf("Input 5~20 characters for password.\n");
		else
			break;
	}
	FILE *fp;
	fp = fopen(USER_DB, "a+");
	fprintf(fp, "%s %s\n", new_id, new_pw);
	fclose(fp);
	user_cnt++;
	strcpy(user_DB[user_cnt].id, new_id);
	strcpy(user_DB[user_cnt].pw, new_pw);

	return 1;
}
