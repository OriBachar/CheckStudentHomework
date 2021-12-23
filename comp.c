#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main (int argc, char* argv[])
{
	char* filename1 = argv[1];

	char* filename2 = argv[2];

	char buffer1[2];

	char buffer2[2];

	int cread1 = 0;

	int cread2  = 0;

	int fd1 = open(filename1,O_RDONLY);

	char msg[64];

	if(argc > 3)
	{
		perror("Worng naumber of arguments");

		exit(-1);
	}

	if(fd1 < 0)
	{
		sprintf(msg,"Unable to open %s\n",filename1);

		perror(msg);

		exit(-1);
	}

	int fd2 = open(filename2,O_RDONLY);

	if(fd2 < 0)
	{
		sprintf(msg,"Unable to open %s\n",filename2);

		perror(msg);

		close(fd1);

		exit(-1);
	}

	struct stat stat_file1;

	struct stat stat_file2;

	if(stat(filename1,&stat_file1) == -1)
	{
		sprintf(msg,"Eror occurred attempting to stat %s\n",filename1);

		perror(msg);

		close(fd1);

		close(fd2);

		exit(-1);
	}

	if(stat(filename2,&stat_file2) == -1)
	{
		sprintf(msg,"Eror occurred attempting to stat %s\n",filename2);

		perror(msg);

		close(fd1);

		close(fd2);

		exit(-1);
	}

	if(stat_file1.st_size != stat_file2.st_size)
	{
		close(fd1);

		close(fd2);

		exit(1);
	}

	do
	{

		cread1 = read(fd1,buffer1,1);

		cread2 = read(fd2,buffer2,1);

		if(cread1 < 0)
		{
			sprintf(msg,"Unable to read from %s\n",filename1);

			perror(msg);

			close(fd1);

			close(fd2);

			exit(-1);
		}

		if(cread2 < 0)
		{
			sprintf(msg,"Unable to read from %s\n",filename2);

			perror(msg);

			close(fd1);

			close(fd2);

			exit(-1);
		}

		if(buffer1[0] != buffer2[0])
		{
			close(fd1);

			close(fd2);

			exit(1);
		}
	}

	while((cread1 == 1) && (cread2 == 1));

	close(fd1);

	close(fd2);

	exit(2);
}


