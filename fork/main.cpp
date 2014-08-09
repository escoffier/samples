/*************************************************************************
	> File Name: main.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Sat 09 Aug 2014 03:36:38 PM CST
 ************************************************************************/

#include<iostream>
#include<unistd.h>
using namespace std;

int main(int argc, char** argv)
{
	printf("parent = %d, pid = %d\n", getppid(), getpid());
    for(int i = 0; i < 2;++i)
	{
		fork();
		printf("parent = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
	}
	return 0;
}
