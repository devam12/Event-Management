#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<sys/wait.h>
#include<stdbool.h>
#include<time.h>
#include<pthread.h>
#include "myheader.h"
int main(int argc, char *argv[])
{
        printf("                          %s Your Invoice                          \n",argv[1]);
	printf("--------------------------------------------------------------------------\n");
	printf("Name\t Event_name\t\t Date\t\t\t Total_amount\t\t\n");
	printf("%s\t %-20s\t %-20s\t %-20s\t\n",argv[1],argv[2],argv[3],argv[4]);
	printf("--------------------------------------------------------------------------\n");
}
