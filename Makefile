# The compiler used
CC = g++

# The compilation method used
CFLAGS = -c -Wall

test:parser.o http.o test.o
	$(CC) parser.o http.o test.o -o test

parser.o: parser.cpp
	$(CC) $(CFLAGS) parser.cpp

http.o: http.cpp
	$(CC) $(CFLAGS) http.cpp

test.o: test.cpp
	$(CC) $(CFLAGS) test.cpp

clean:
	rm -rf *o test