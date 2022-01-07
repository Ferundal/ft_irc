//
// Created by ferun on 07.01.2022.
//

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	char buff[50];

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
	int fd_output = open("output.txt", O_WRONLY | O_CREAT, S_IRWXO );
	if (fd_output == -1)
	{
		printf("Can't create output file\n");
		return (3);
	}
	int status = 0;
	while ((status = read(fd_input, buff, 1)) != 0)
	{
		write (fd_output, "#define ", 8);
		while ((status != 0 && buff[0] == ' '))
		{
			status = read(fd_input, buff, 1);
		}
		if (status == 0)
			return (4);
		char c;
		int counter = 0;
		while ((status = read(fd_input, &c, 1)) != 0 && c != ' ')
		{
			buff[++counter] = c;
		}
		if (status == 0)
			return (5);
		buff[counter + 1] = '\0';
		printf ("%s\n", buff);
		write (fd_output, buff, counter);
		while ((status = read(fd_input, buff, 1)) != 0 && buff[0] == ' ')
	{}
		if (status == 0)
			return (6);
		counter = 0;
		while ((status = read(fd_input, &c, 1)) != 0 && c != '\n')
		{
			buff[++counter] = c;
		}
		if (counter == 0 && status == 0)
			return (7);
		buff[counter + 1] = '\0';
		write (fd_output, buff, counter);
		write (fd_output, "\n", 1);
	}
}