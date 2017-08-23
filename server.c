/******************************************************************************
 *
 *  File Name : server.c
 *
 *  File creation : 2017-08-16
 *  Function : Chatting server
 *
 ******************************************************************************/

#include "header.h"

void *clnt_connection(void *arg);
void send_message(char *message, int len);

int clnt_number = 0;
int clnt_socks[MAX_CONNECT];

pthread_mutex_t mutex;

int main(int argc, char **argv)
{
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	pthread_t thread;

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if(pthread_mutex_init(&mutex, NULL))
		exit_error("mutex init error");
		
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		exit_error("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		exit_error("bind() error");
	
	if(listen(serv_sock, 5) == -1)
		exit_error("listen() error");

	printf("Chatting Server Inited.\n");
	printf("Wating for client login.\n");	
	
	while(1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
       
		pthread_mutex_lock(&mutex);
       
		clnt_socks[clnt_number++] = clnt_sock;
		pthread_mutex_unlock(&mutex);
		
		pthread_create(&thread, NULL, clnt_connection, (void *)(intptr_t) clnt_sock);
		printf(" [%s] (ip: %s) login.\n", user_DB[user_index].id, inet_ntoa(clnt_addr.sin_addr));
	}
	return 0;
}

void *clnt_connection(void *arg)
{
	int clnt_sock = (intptr_t) arg;
	int str_len=0;
	char message[BUFSIZE];
	int i;

	while((str_len = read(clnt_sock, message, sizeof(message))) != 0)
		send_message(message, str_len);
  
	pthread_mutex_lock(&mutex);
	for(i=0; i<clnt_number; i++)
	{ 
		if(clnt_sock == clnt_socks[i])
		{
			for(; i < clnt_number-1; i++)
				clnt_socks[i] = clnt_socks[i+1];
			break;
		}
	}
	clnt_number--;
  
	pthread_mutex_unlock(&mutex);

	close(clnt_sock);
	return 0;
}

void send_message(char *message, int len)
{
	int i;
	pthread_mutex_lock(&mutex);
  
	for(i=0; i<clnt_number; i++)
		write(clnt_socks[i], message, len);
	pthread_mutex_unlock(&mutex);
}
