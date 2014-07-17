/****************************************************************
            Copyright (C) 2014 All rights reserved.
					      									  
    > File Name:         < get_sca60a.c >
    > Author:            < Shawn Guo >
    > Mail:              < iguoxiaopeng+code@gmail.com >
    > Created Time:      < 2014/06/05 >
    > Last Changed: 
    > Description:
****************************************************************/


#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE_NAME 	"data.txt"
#define OUTPUT_FILE_NAME	"out.txt"

int main()
{
	FILE *in_fp = NULL, *out_fp = NULL;
	char c;
	float input_x, input_y, output_x, output_y;
	int i = 0;
	
	if((in_fp = fopen(INPUT_FILE_NAME, "r")) == NULL)
		fprintf(stderr, "Error to open file %s", INPUT_FILE_NAME);
	if((out_fp = fopen(OUTPUT_FILE_NAME, "w+")) == NULL)
		fprintf(stderr, "Error to open file %s", OUTPUT_FILE_NAME);
	
	//Input file format:
	//3282, In:7.74/-0.99, Out:-7.74/0.99,M1- 	
	while(1)
	{
		if(feof(in_fp)) break;
		c = fgetc(in_fp);
		if(c == '/')
		{
			fscanf(in_fp, "%f", &input_y);
			if(++i % 2)
				fprintf(out_fp, "%f\t", input_y);
			else
				fprintf(out_fp, "%f\n", input_y);
		}
	}
	fclose(in_fp);
	fclose(out_fp);	

    return 0;
}

