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
const char* eventfile = "event.txt";
void view_event()
{
	FILE *in_file = fopen(eventfile, "r");

	struct stat sb;
	stat(eventfile, &sb);
	char *file_contents = malloc(sb.st_size);
	int i=0;
	while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
		i++;
        	printf("%d > %s\n", i,file_contents);
    	}
}