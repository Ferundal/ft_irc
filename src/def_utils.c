//
// Created by ferun on 07.01.2022.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv)
{
	char err_code[50];
	char err_txt[50];

	if (argc != 2)
	{
		printf("One input file needed\n");
		return (1);
	}
	int fd_input = open(*(argv + 1), O_RDONLY);
	if (fd_input == -1)
	{
		printf("Input file is not correct\n");
		return (2);
	}
	int fd_output = open("output.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
	if (fd_output == -1)
	{
		printf("Can't create output file\n");
		return (3);
	}
	int status = 0;
	while ((status = read(fd_input, err_code, 1)) != 0)
	{
		while (status != 0 && ((err_code[0] == ' ') || isprint(err_code[0]) == 0))
		{
			status = read(fd_input, err_code, 1);
		}
		if (status == 0)
			return (4);
		char c;
		int counter = 0;
		while ((status = read(fd_input, &c, 1)) != 0 && c != ' ')
		{
			if (isprint(c) != 0)
				err_code[++counter] = c;
		}
		if (status == 0)
			return (5);
		err_code[++counter] = '\0';
		while ((status = read(fd_input, err_txt, 1)) != 0 && ((err_txt[0] == ' ') || isprint(err_txt[0]) == 0))
	{}
		if (status == 0)
			return (6);
		counter = 0;
		while ((status = read(fd_input, &c, 1)) != 0 && c != '\n')
		{
			if (isprint(c) != 0)
				err_txt[++counter] = c;
		}
		if (status == 0)
			return (5);
		err_txt[++counter] = '\0';
		write (fd_output, "#define ", 8);
		write (fd_output, err_txt, strlen(err_txt));
		write (fd_output, " ", 1);
		write (fd_output, err_code,  strlen(err_code));
		write (fd_output, "\n", 1);
	}
}