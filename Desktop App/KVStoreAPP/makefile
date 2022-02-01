#
#
#	Makefile for test programs
#
#


all: kvstoreapp

kvstoreapp: main.o KVDataBase.o ConsoleExec.o ServerOBJ.o IPCExecution.o
	g++ -o kvstoreapp main.o KVDataBase.o ConsoleExec.o ServerOBJ.o IPCExecution.o -lleveldb



clean:
	rm -f *.o *~ kvstoreapp


