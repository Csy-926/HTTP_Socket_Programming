# The compiler used
CC = g++

# The compilation method used
CFLAGS = -c -Wall

all: my_client my_server_non my_server_p

my_client: parser.o http.o file.o client.o
	$(CC) parser.o http.o file.o client.o -o my_client

my_server_p: parser.o http.o file.o server_persistent.o
	$(CC) parser.o http.o file.o server_persistent.o -o my_server_p

my_server_non: parser.o http.o file.o server_nonpersistent.o
	$(CC) parser.o http.o file.o server_nonpersistent.o -o my_server_non

test:parser.o http.o file.o test.o
	$(CC) parser.o http.o file.o test.o -o test

parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp

http.o: http.cpp
	$(CC) $(CFLAGS) http.cpp

file.o: file.cpp
	$(CC) $(CFLAGS) file.cpp

server_persistent.o: server_persistent.cpp
	$(CC) $(CFLAGS) server_persistent.cpp

server_nonpersistent.o: server_nonpersistent.cpp
	$(CC) $(CFLAGS) server_nonpersistent.cpp

client.o: client.cpp
	$(CC) $(CFLAGS) client.cpp

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

clean:
	rm -rf *o test my_server_p my_server_non my_client