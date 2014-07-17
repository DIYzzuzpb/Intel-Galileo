/****************************************************************
            Copyright (C) 2014 All rights reserved.
					      									  
    > File Name:         < get_random_data.c >
    > Author:            < Shawn Guo >
    > Mail:              < iguoxiaopeng+code@gmail.com >
    > Created Time:      < 2014/06/10 >
    > Last Changed: 
    > Description:
****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME 	"random_data.txt"
#define OUTPUT_FILE_NAME	"out.txt"
#define COUNT_BASE 10

int main()
{
	FILE *in_fp = NULL, *out_fp = NULL;
	char c;
	float input, output;
	int i = 0;
	
	if((in_fp = fopen(INPUT_FILE_NAME, "r")) == NULL)
		fprintf(stderr, "Error to open file %s", INPUT_FILE_NAME);
	if((out_fp = fopen(OUTPUT_FILE_NAME, "w+")) == NULL)
		fprintf(stderr, "Error to open file %s", OUTPUT_FILE_NAME);
	
	//Input file format:
	//一行一个float
	while(1)
	{
		if(feof(in_fp)) break;
		fscanf(in_fp, "%f", &input);
		if(++i % COUNT_BASE == 0)
			printf("%.3f,\n", input);
	}
	fclose(in_fp);
	fclose(out_fp);	

    return 0;
}

