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
const char* billfile = "BookingEvent.txt";

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
void * store_data(void *arg)
{
	printf("\n\tI am thread1 and I'm storing customer details in database my id %u is starting \n", (unsigned int)pthread_self());
	struct booking *thread_p = arg;
	//printf("Thread with multiple param : %s %s %s %s ",thread_p->cname,thread_p->event,thread_p->finaldate,thread_p->totalbill_arr);
	//create a file
	//printf("In store data");
	fptr2=fopen(billfile,"a");
	
	if(fptr2==NULL)
	{
		printf("Error in opening file");
		exit(1);
	}
	else
	{
		if (fptr2)
		{
			fseek (fptr2, 0, SEEK_END);
			int size = ftell(fptr2);
			//printf("Size of the file in bytes: %dn", size);
			if(size==0)
			{
				fprintf(fptr2,"Name\t Event Name\t Date\t Total_Amount\n");
			}
			fprintf(fptr2,"\n");
			fprintf(fptr2,"%s\t%s\t%s\t%s",thread_p->cname,thread_p->event,thread_p->finaldate,thread_p->totalbill_arr);
		}
		

		fclose(fptr2);
		//printf("\nBill is ready\n");
	}
	printf("\n\tDone with storing details of customers now thread1 %u is exiting \n", (unsigned int)pthread_self());
	pthread_exit(0);
}
