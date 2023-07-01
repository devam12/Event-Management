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
int fd1[2],fd2[2];
const char* filename = "event.txt";
int child_status;
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


void childHandler()  /* Executed if the child dies before the parent */ 
{
	int  childPid, childStatus;
	childPid = wait(&childStatus);   /* Accept child’s termination code */
	printf("\nSorrry!! It seems like you entered some invalid details!!!");
	printf("\nChild %d terminated \n", childPid);
	exit(/* EXIT SUCCESS */  0); 
}

void alarmHandler() 
{
	printf("\nIt took too long to enter the input \n");
	//we can also use kill command instead of exit
	exit(-1);
}
int alarmFlag = 0;	/* Global alarm flag */
void (*oldhandler)();

int main()
{
	//DECLARE VARIABLE
	char cust_name[50];
	int status;
	int childPid;
	
	//CREATE PIPE 
	if (pipe(fd1) == -1) 
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	if (pipe(fd2) == -1) 
	{
		fprintf(stderr, "Pipe Failed");
		return 1;
	}
	
	//INSTALL SINGNAL HANDLERS
	oldhandler = signal( SIGALRM, alarmHandler );

	//ENTER IN OUR EVENT MANAGMENT SYSTEM
	printf("\n------------------------------------------------------------------\n");
	printf("               Welcome to our Event Company                ");
	printf("\n------------------------------------------------------------------\n");
	
	//ENTER CUSTOMER NAME WITHIN 10 SECOND
	printf("\nEnter your name : ");
	alarm(10);
	gets(cust_name);
	alarm(0);

	//PRINT WELCOME MESSAGE 
	printf("\n------------------------------------------------------------------\n");
	printf("\nWelcome %s to our Event Company. Book your event here\n",cust_name);
	printf("\n------------------------------------------------------------------\n");
	 
	
	signal( SIGINT, childHandler );
	
	//CREATE CHILD PROCESS
	childPid = fork();	
	if(childPid < 0)			
	{
		perror("Fork error!!");
		exit(1);
	}
	else if(childPid == 0) //CHILD PROCESS
	{
		//DECLARE VARIABLE
		int choice;
		char choice_arr[5];
		char eventname_arr[20];
		int cur_year=2022;
		int date,month,year;
		char arr[15],finaldate[20],userdate[10],usermonth[10],curr_date[10],curr_month[10];
		int capacity=0;
		char cap_arr[5];
		
		//CLOSE PIPE FOR UNI-DIRECTION
		close(fd1[0]);
		close(fd2[1]);
		
		//PPRINT EVENTS 
		view_event(); 
		
		//ENTER EVENT ID AND WRITE TO PARENT
		printf("\nEnter event id : ");
		scanf("%d",&choice);
		snprintf(choice_arr,10,"%d",choice);
		write(fd1[1], choice_arr, strlen(choice_arr) + 1);
		
		
		//READ EVENT NAME FROM PARENT AND PRINT
		read(fd2[0], eventname_arr, 20);
		printf("\n------------------------------------------------------------------\n");
		printf("\nYour selected event is %s\n",eventname_arr);
		printf("\n------------------------------------------------------------------\n");
		//READ FULLDATE FROM USER 
		do
		{
			printf("\nEnter Date which you want to book(Enter in DD format)	:");
			scanf("%d",&date);
		}while(!(date>0 && date<=31));
		do
		{
			printf("Enter month which you want to book(Enter in MM format)	:");
			scanf("%d",&month);
		}while(!(month>0 && month<=12));
		do
		{
			printf("Enter year which you want to book(Enter in YYYY Format)	:");
			scanf("%d",&year);
		}while(cur_year!=year);
		
		
		//GET CURRENT DATE FROM SYSTEM
		time_t t;  		
		t = time(NULL);
		struct tm tm = *localtime(&t);
		//printf("\nCurrent Date      : %d-%d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
     
		//CONVERT DATE
    	snprintf(arr,10,"%d",date);
		strcpy(finaldate,arr);
		strcpy(userdate,arr);
		strcat(finaldate,"-");
    
    	snprintf(arr,10,"%d",month);
		strcat(finaldate,arr);
		strcpy(usermonth,arr);
		strcat(finaldate,"-");
		
		snprintf(arr,10,"%d",year);
		strcat(finaldate,arr);
	
		snprintf(&curr_date,10,"%d",tm.tm_mday);
		snprintf(&curr_month,10,"%d",tm.tm_mon+1);
		
		int iusermonth=atoi(usermonth);
		int iuserdate=atoi(userdate);
		int icurrmonth = atoi(curr_month);
		int icurrdate = atoi(curr_date);
		printf("\n--------------------------------------------------------\n");
		printf("Your Entered Date   : %s ",finaldate);
		
		char flag[10]="False";
    	if(icurrmonth < iusermonth)
    	{
			strcpy(flag,"True");
   		}
    	else if(icurrmonth==iusermonth)
    	{
   			if(icurrdate <= iuserdate)
			{
				strcpy(flag,"True");
   		   	}
    	}
		
		//SEND FLAG VALUE FOR PROCESS CONFIRMATION
		write(fd1[1], flag, strlen(flag) + 1);
		sleep(5);
		
		//SEND FINAL DATE IF CHILD PROCESS ALIVE
		write(fd1[1], finaldate, strlen(finaldate) + 1);
		
		
		//READ PERSONE CAPACITY FROM USER AND PASS PARENT PROCESS
		printf("\n\nEnter Total Persons : ");
		scanf("%d",&capacity);
		while(capacity<50 || capacity>200)
		{
			printf("\nYou Entered Invalid Persons Please Enter Between 50 to 200 Persons");
			scanf("%d",&capacity);
		}
		snprintf(cap_arr,10,"%d",capacity);
		write(fd1[1], cap_arr, strlen(cap_arr) + 1);
		
		
		//READ BILL AMOUNT 
		char totalbill_arr[10];
		read(fd2[0],totalbill_arr,10);
		printf("\nYour Bill Amount is : %s\n",totalbill_arr);
		
		//READ CONFIRMATION
		char confirmation[5];
		printf("\nBook Event (YES/no) : ");
		scanf("%s",&confirmation);
		write(fd1[1], confirmation, strlen(confirmation) + 1);
		int child_status;
		sleep(5);
		child_status = execl("./printbill.out", "./printbill.out", cust_name,eventname_arr,finaldate,totalbill_arr,NULL);
		
	
		
		//sleep(4);
		
	}
	else	
	{
		char eventid_arr[5];
		char eventname_arr[20];
		char flag[10];
		char records1[1024];
		int row=1;
		char *token;
		char line[20][100];
		pthread_t threadID;
		int c;
		
		//CLOSE THE PIPE FOR UNI-DIRECTION
		close(fd1[1]);
		close(fd2[0]);
		
		//READ EVENT ID FROM CHILD
		read(fd1[0], eventid_arr, 5);
		int val = atoi(eventid_arr);
		
		//FIND EVENT NAME BASED ON EVENT-ID FROM EVENT.TXT FILE 
		FILE *mainfile = fopen(filename, "r");
		while(fgets(records1,sizeof(records1),mainfile))
		{
			strcpy(line[row],records1);
			row++;
		}
		char s[2]=",";   
	
		//GET THE FIRST TOKEN
		token = strtok(line[val], s);
		
		//WALK THROUGH OTHER TOKENS IN LINE
		while( token != NULL && c!=1 ) {
			c++;
		       token = strtok(NULL, s);
		}
		
		//COPY TOKEN VALUE IN CHAR ARRAY
		strcpy(eventname_arr,token);
		//printf("token value : %s",token);
		write(fd2[1], eventname_arr, strlen(eventname_arr) + 1);		
		
		
		//READ FLAG COMPPARE
		read(fd1[0], flag, 10);
		if(strcmp(flag,"False")==0)
		{
			kill(childPid, SIGINT );
		}
		else 
		{
			char cap_arr[5];
			char finaldate[20];
			char split[2]=",";
			row=0;
			
			//READ FINAL DATE FROM USER
			read(fd1[0], finaldate, 20);
			
			//READ PERSONE CAPACITY FROM USER
			read(fd1[0], cap_arr, 5);
			int capacity= atoi(cap_arr);
			
			FILE *mainfile = fopen(filename, "r");
			while(fgets(records1,sizeof(records1),mainfile))
			{
				strcpy(line[row],records1);
				row++;
			}
			token = strtok(line[val-1],split);
			
			//walk through other tokens 
			while( token != NULL && c!=3 ) {
				c++;
				token = strtok(NULL, split);
			}
			
			//GENRATE BILL PRICE
			char p[5];
			strcpy(p,token);
			int price = atoi(p);
			printf("\nOne persone price is		: %d",price);	
			printf("\nYour entered capacity		: %d\n",capacity);
			int totalbill = price*capacity;
			
			//SEND CHILD PROCESS TOTALBILL AMOUNT
			char totalbill_arr[10];
			snprintf(totalbill_arr,10,"%d",totalbill);
			write(fd2[1], totalbill_arr,strlen(totalbill_arr) + 1);
			
			//READ CONFIRMATION FROM CHILD
			char confirmation[5];
			read(fd1[0],confirmation, 5);
			
			//CHECK CONFIRMATION STATUS IF YES BOOK EVENT OTHERWISE KILL CHILD
			if(strcmp(confirmation,"YES")==0)
			{
				int err;
				pthread_t threadID1,threadID2;
				int *threadReturnID;
				
				printf("\nMain thread is waiting while its sub-thread are working Main thread id %u is starting \n", (unsigned int)pthread_self());
				strcpy(b1.cname,cust_name);
				strcpy(b1.event,eventname_arr);
				strcpy(b1.finaldate,finaldate);
				strcpy(b1.totalbill_arr,totalbill_arr);
				err = pthread_create (&threadID1, NULL, generate_bill, (void *)&b1);
				if (err != 0)
					printf("cant create thread: %s\n", strerror(err));
				//second thread
				err = pthread_create (&threadID2, NULL, store_data, (void *)&b1);
				if (err != 0)
					printf("cant create thread: %s\n", strerror(err));
				//generate_bill(cust_name, eventname_arr,finaldate,totalbill_arr);
				//store_data(cust_name, eventname_arr,finaldate,totalbill_arr);
				err = pthread_join(threadID1,NULL);
				if (err != 0)
					printf("cant join with thread1: %s\n", strerror(err));
				err = pthread_join(threadID2,NULL);
				if (err != 0)
					printf("cant join with thread1: %s\n", strerror(err));
				printf("\nDone with all the work main thread id %u is exiting \n\n", (unsigned int)pthread_self());
				wait(&child_status);
				printf("\nYour Event is booked Thank you visit again!!!!\n ");
			}
			else
			{
				kill(childPid, SIGINT );
			}
			
		}
		wait(&status);			
	}
	return 1;
}
