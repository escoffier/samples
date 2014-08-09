
objects = main.o
cc = g++

httpserver : $(objects)
	g++ -o sqlitest $(objects) -lpthread -lsqlite3 -g
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
