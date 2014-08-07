
objects = main.o
cc = g++

httpserver : $(objects)
	g++ -o sqlitest -g $(objects) -lpthread lsqlite3
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
