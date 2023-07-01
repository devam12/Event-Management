#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void main()
{
	char user[100];
	char records[1024];
	char record[1024];
	printf("\n");
	char* pos;
	int i = 0,j = 0, cnt = 0;
	
	int k = 0, row = 0, wrd = 0, wrds = 0, letters = 0, letter = 0;
	cnt = 0;
	char rec[3][100],recs[40][5][100];
	
	char ch;
	int lines;//to get total records in file
	char temp[2],fileName[10],line[40][1024];
	//char mainfile[15] = "Product.txt";
	const char* mainFile = "Product.txt";
	FILE *mainfile = fopen(mainFile, "r");

	while(fgets(records,sizeof(records),mainfile))
	{
		/*if(cnt == 0)
		{
			cnt++;
			continue;
		}*/
				printf("\nRow===%d",row);
		printf("%s",records);	
		strcpy(line[row],records);
		printf("L==%s",line[row]);
		row++;
		printf("\n\nHELLO\n\n");
		printf("\nRow++===%d",row);
			printf("\n\nAFTER HELLO\n\n");
		//if(row==5)
		//{
		//	break;
		//}
	}

	//fetching words from line
	printf("Row===%d",row);
	for(i = 0;i<row;i++)
	{
		printf("I==%d",i);
		for(k = 0;k<strlen(line[i]);k++)
		{
			printf("%c",line[i][k]);
			if(line[i][k] == '\n' || line[i][k] == ' ' || line[i][k] == '\0' || line[i][k] == '\t')
			{
				recs[i][wrd][letter] = '\0';
				wrd++;
				letter = 0;
				printf("IN IF");
			}
			else
			{
				recs[i][wrd][letter] = line[i][k];
				//printf("3D array %s",recs[i][wrd][letter]);
				letter++;
			}
		
		}
			printf("\n");
	}
}