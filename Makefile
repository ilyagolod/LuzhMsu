CC=g++ -std=c++14

all: test

#соберем все-все
record.o: record.cpp record.h
	$(CC) -c record.cpp
	
test.o: test.cpp record.h RBTree.h server.h client.h
	$(CC) -c test.cpp  

tester: record.o test.o 
	$(CC) -o tester test.o record.o

test: tester
	./tester


#для файла начало
make_file: filemaker
	./filemaker

filemaker: filemaker.o
	$(CC) -o filemaker filemaker.o

filemaker.o: filemaker.cpp
	$(CC) -c filemaker.cpp
#для файла конец

clean: 
	rm -rf *.o tester filemaker