 ###############################################################################
 #
 #  File Name : Makefile
 #
 #  File creation : 2017-08-16
 #
 ###############################################################################

CC = gcc
CFLAGS = 
CTAGS = -pthread
TARGET = server client
HEADER = header.h
SERV_SRC = server.c exit_error.c
CLNT_SRC = client.c exit_error.c user_man.c

all : $(TARGET)

server : $(SERV_SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(SERV_SRC) $(CTAGS)

client : $(CLNT_SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $@ $(CLNT_SRC) $(CTAGS)

clean :
	rm $(TARGET)

cleandb :
	rm user_db.txt
