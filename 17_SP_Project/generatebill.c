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
//const char* filename = "event.txt";
//const char* billfile = "BookingEvent.txt";

FILE *fptr;
FILE *fptr2;
struct booking
{
	char cname[50];
	char event[50];
	char finaldate[20];
	char totalbill_arr[10];
};
struct booking b1;
void * generate_bill(void *arg)
{
	printf("\n\tI am thread2 and I'm generating the bill of customer my id %u is starting \n", (unsigned int)pthread_self());
	struct booking *thread_p = arg;
	//printf("Thread with multiple param : %s %s %s %s ",thread_p->cname,thread_p->event,thread_p->finaldate,thread_p->totalbill_arr);
	//create a file	
	char customer_filename[50];
	stpcpy(customer_filename,thread_p->cname);
	strcat(customer_filename,".txt");
	fptr=fopen(customer_filename,"a");
	
	if(fptr==NULL)
	{
		printf("Error in opening file");
		exit(1);
	}
	else
	{
		fseek (fptr, 0, SEEK_END);
			int size = ftell(fptr);
			//printf("Size of the file in bytes: %dn", size);
			if(size==0)
			{
				fprintf(fptr,"Name\t Event Name\t Date\t Total_Amount\n");
			}
			fprintf(fptr,"\n");		
		fprintf(fptr,"%s\t%s\t%s\t%s",thread_p->cname,thread_p->event,thread_p->finaldate,thread_p->totalbill_arr);
		fclose(fptr);
	}
	printf("\n\tDone with generating bill now thread2 id %u is exiting \n", (unsigned int)pthread_self());
	pthread_exit(0);
}
