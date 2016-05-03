/*
MINDSHATTER
All purpose brainfuck interpreter
Copyright Adam Decker 2016
Version 1.1.0
*/

#define _CRT_SECURE_NO_WARNINGS
#define bool int
#define true 1
#define false 0
#define mode int
#define file 0
#define commandline 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool end_loop(int *i);
bool process_line(char line[]);
bool increment_ptr();
bool decrement_ptr();
bool increment_val();
bool decrement_val();
bool output_byte();
bool input_byte();
bool start_loop(int *i);
bool end_loop(int *i);
bool process_cl_args(int argc, char *argv[], char infile[], char outfile[], mode *input, mode *output);
bool preprocess_line(char line[], int lineNumber);


char data[30000] = { '\0' };
int ptr = 0;
mode input = commandline;
mode output = commandline;
FILE *fout;
int *loop_stack;
int stack_size;
int stack_iterator = 0;


void show_usage(char name[])
{
	fprintf(stderr, "Usage: ");
	fprintf(stderr, name, "%s");
	fprintf(stderr,
		"Options:\n"
		"\t-i\t\tSpecify the input destination path\n"
		"\t-o,--destination DESTINATION\tSpecify the output destination path\n"
		"%s");
}


int main(int argc, char *argv[])
{
	loop_stack = (int *)malloc(10 * sizeof(int));
	stack_size = 10;
	char infile[80] = "\0";
	char outfile[80] = "\0";

	if (!process_cl_args(argc, argv, infile, outfile, &input, &output))
		return 1;

	printf( "MindShatter v1.0", "%s");
	//Initialize brainfuck array

	bool dead = false;
	int lineNumber = 1;
	char *line;
	line = (char *)malloc(80 * sizeof(char));
	
	while (!dead)
	{
		if (input == commandline)
		{
			printf("\n>>>", "%s");
			scanf("%s", line);
			if(preprocess_line(line, lineNumber))
				process_line(line);
			lineNumber++;
		}
		else
		{
			FILE *fin= fopen(infile, "r");
			if (fin == NULL)
			{
				fprintf( stderr, "Error opening input file\n");
				return 1;
			}

			while (!feof(fin))
			{
				fscanf(fin, "%s", line);
				if (preprocess_line(line, lineNumber))
					process_line(line);
				lineNumber++;
			}

			fclose(fin);
			dead = true;
		}
	}
}


bool process_cl_args(int argc, char *argv[], char infile[], char outfile[], mode *input, mode *output)
{
	for (int i = 1; i < argc; ++i)
	{
		char arg[80];
		strcpy(arg, argv[i]);
		if (strcmp(arg, "-i") == 0)
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				strcpy(infile, argv[++i]); // Increment 'i' so we don't get the argument as the next argv[i].
				input = file;
			}
			else
			{ // There was no argument to the destination option.
				fprintf( stderr, "--input file option requires one argument.", "%s");
				return false;
			}
		}
		else if (strcmp(arg, "-o") == 0)
		{
			if (i + 1 < argc)
			{ // Make sure we aren't at the end of argv!
				outfile = argv[++i]; // Increment 'i' so we don't get the argument as the next argv[i].
				output = file;
			}
			else
			{ // There was no argument to the destination option.
				fprintf(stderr, "--output file option requires one argument.", "%s");
				return false;
			}
		}
	}
	return true;
}

bool process_line(char line[])
{
	int i = 0;
	int length = strlen(line);

	while(i < length)
	{
		switch (line[i])
		{
		case '>':
			if (!increment_ptr())
				return false;
			break;
		case '<':
			if(!decrement_ptr())
				return false;
			break;
		case '+':
			if(!increment_val())
				return false;
			break;
		case '-':
			if (!decrement_val())
				return false;
			break;
		case '.':
			if(!output_byte())
				return false;
			break;
		case ',':
			if(!input_byte())
				return false;
			break;
		case '[':
			if(!start_loop(&i))
				return false;
			break;
		case ']':
			if (!end_loop(&i))
				return false;
			break;
		default:
			break;
		}
		
		i++;
	}

	return true;
}


bool preprocess_line(char line[], int lineNumber)
{
	int loops = 0;
	int length = strlen(line);

	for (int i = 0; i < length; i++)
	{
		switch (line[i])
		{
			case '[':
				loops++;
				break;
			case ']':
				loops--;
				break;
			default:
				break;
		}
	}

	if (loops != 0)
	{
		char str[80];
		printf("Error at line ", "%s");
		sprintf(str, "%d", lineNumber);
		printf(str);
		printf(": Missing \"[\" or \"]\"", "%s");
		return false;
	}

	return true;
}


bool increment_ptr()
{
	if (ptr < 30000)
	{
		ptr++;
	}
	else
		ptr = 0;

	return true;

}

bool decrement_ptr()
{
	if (ptr > 0)
		ptr--;
	else
		ptr = 30000;

	return true;
}

bool increment_val()
{
	data[ptr]++;
	return true;
}

bool decrement_val()
{
	data[ptr]--;
	return true;
}

bool output_byte()
{
	char str[2] = "\0";
	str[0] = (char)data[ptr];
	if (output == commandline)
	{
		printf( str );
	}

	else
	{ 

		fprintf(fout, "%s", str);
	}
	return true;
}

bool input_byte()
{
	if (input == commandline)
	{
		printf(":");
		char string[2];
		fflush(stdin);
		scanf("%c", string);
		data[ptr] = string[0];

	}
	return true;
}

bool start_loop(int *i)
{
	int j = *i;

	//Make sure there's space on the stack
	if (!(stack_iterator < stack_size))
	{
		//Expand the stack first if there's no space
		stack_size += 10;
		loop_stack = (int *)malloc(stack_size * sizeof(int));

	}

	//Add it to the stack and increment stack iterator
	loop_stack[stack_iterator] = *i;
	stack_iterator++;

	return true;
}

bool end_loop(int *i)
{
	if (data[ptr] == '\0')
		stack_iterator--;
	else
		*i = loop_stack[stack_iterator - 1];
	return true;
}

