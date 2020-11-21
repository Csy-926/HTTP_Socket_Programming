# The compiler used
CC = g++

# The compilation method used
CFLAGS = -c -Wall

my_server:parser.o http.o file.o server.o
	$(CC) parser.o http.o file.o server.o -o my_server

test:parser.o http.o file.o test.o
	$(CC) parser.o http.o file.o test.o -o test

parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp

http.o: http.cpp
	$(CC) $(CFLAGS) http.cpp

file.o: file.cpp
	$(CC) $(CFLAGS) file.cpp

server.o: server.cpp
	$(CC) $(CFLAGS) server.cpp

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

clean:
	rm -rf *o test my_server