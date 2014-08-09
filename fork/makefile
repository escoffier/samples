
objects = main.o
cc = g++

httpserver : $(objects)
	g++ -o test $(objects) -g
#main.o : main.cpp
#	cc -c main.cpp
	
.PHONY : clean
clean :
	rm edit $(objects)
