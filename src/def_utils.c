//
// Created by ferun on 07.01.2022.
//

#include <stdio.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	char err_code[80];
	char err_txt[80];
	if (argc != 2)
	{
		printf("One input file needed\n");
		return (1);
	}
	FILE *fd_input = fopen(*(argv + 1), "r");
	if (fd_input == NULL)
	{
		printf("Input file is not correct\n");
		return (2);
	}
	FILE *fd_output = fopen("output.txt", "w+");
	if (fd_output == NULL)
	{
		printf("Can't create output file\n");
		return (2);
	}
	while (fscanf(fd_input, " %s %s\n", err_code, err_txt) == 2) {
		printf("%s %s\n", err_code, err_txt);
		fprintf(fd_output, "#define %s %s\n", "1", "2");
	}
}