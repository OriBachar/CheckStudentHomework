#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/wait.h>

#define MAX_PATH 50

void readLine(int fd, char* arr, int size);

void appendS(char* str1, char* str2);

int readSub(char* path, char* inputFile, char* outputFile, char* folder);

int execFork(char** argsv);

int wirteResult(char* name, char* result);

int stringLen(char* str);

int main(int argc, char* argv[])
{

	int configFD, resultFD;

	char msg[64];

	char studentPath[MAX_PATH], inputPath[MAX_PATH], examplePath[MAX_PATH];

	if (argc != 2)
	{
		perror("Worng namber of arguments");
		exit(-1);
	}

	configFD = open(argv[1], O_RDONLY);

	if (configFD < 0)
	{
		perror("Unable to open config.txt");

		exit(-1);
	}

	resultFD = open("result.csv", O_RDWR | O_CREAT | O_TRUNC, 0666);

	if (resultFD < 0)
        {
                perror("Unable to open result.csv");

		close(configFD);

                exit(-1);
        }

	close(resultFD);

	readLine(configFD, studentPath, MAX_PATH);

	readLine(configFD, inputPath, MAX_PATH);

	readLine(configFD, examplePath, MAX_PATH);

	DIR* dirStu = opendir(studentPath);

	if (dirStu  == NULL)
	{
		perror("Unable to open student folder");

		close(configFD);

		exit(-1);
	}

	struct dirent* dirSt;

	while (dirSt = readdir(dirStu))
		if (readSub(studentPath, inputPath, examplePath, dirSt->d_name) < 0)
		{
			close(configFD);

			exit(-1);
		}

	char* catArg[] = {"cat", "result.csv", NULL};

	execFork(catArg);

	char* rmArg[] = {"rm", "output.txt",NULL};

 	execvp(rmArg[0], rmArg);

	closedir(dirStu);

	close(configFD);

	exit(0);
}

void readLine(int fd, char* arr, int size)
{
	char ch;

	int i = 0;

	while (read(fd, &ch, 1) > 0 && i < size)
	{
		if(ch == '\n')
			break;

		arr[i++] = ch;
	}

	arr[i] = '\0';
}

void appendS(char* str1, char* str2)
{
	int i = 0, j = 0;

	while (str1[i] != '\0')
		 i++;

	while (str2[j] != '\0')
	{
		str1[i] = str2[j];

		i++;

		j++;
	}

	str1[i] = '\0';
}

int readSub(char* path, char* inputFile, char* outputFile, char* folder)
{
	struct stat Stat;

	char curPath[MAX_PATH] = "";

	char curFilePath[MAX_PATH] = "";

	char curDesPath[MAX_PATH] = "";

	int returnValue, input, output;

	int  keyBoard = dup(0);

	int screen = dup(1);

	if (folder[0] == '.')
		return 0;


	appendS(curPath, path);

	appendS(curPath, folder);

	if (stat(curPath, &Stat) < 0)
	{
		perror("Unable to open stat\n");

		exit(-1);
	}

	if (!S_ISDIR(Stat.st_mode))
		return 0;

	appendS(curPath, "/");

	appendS(curFilePath, curPath);

	appendS(curFilePath,folder);

	appendS(curFilePath, ".c");

	appendS(curDesPath, curPath);

	appendS(curDesPath, "main.out");

	char* gccArg[] = {"gcc", curFilePath, "-o", curDesPath, NULL};

	if (execFork(gccArg) < 0)
	{
		perror("Unable to compile the files\n");

		exit(-1);
	}

	input = open(inputFile, O_RDONLY);

	output = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

	if (input < 0 || output < 0)
	{
		perror("Unable to open file\n");

		exit(-1);
	}

	dup2(input,0);

	dup2(output, 1);

	char command[MAX_PATH] = "";

	appendS(command, "./students/");

	appendS(command, folder);

	appendS(command, "/main.out");

	char* runArg[] = {command, NULL};

	if (execFork(runArg) < 0 )
	{
		perror("Unable to executing main.out\n");

		close(input);

		close(output);

		exit(-1);
	}

	dup2(keyBoard, 0);

	dup2(screen, 1);

	char* compArg[] = {"./comp.out", "output.txt", outputFile, NULL};

	returnValue = execFork(compArg);

	if (returnValue < 0)
	{
		perror("Unable to compre the files\n");

		close(input);

		close(output);

		exit(-1);
	}

	if (returnValue == 2)
		if (wirteResult(folder, "100") < 0)
		{
			perror("Unable to write to csv file\n");

			close(input);

			close(output);

			exit(-1);
		}

	 if (returnValue == 1)
                if (wirteResult(folder, "0") < 0)
                {
                        perror("Unable to write to csv file\n");

                        close(input);

                        close(output);

                        exit(-1);
                }

	close(input);

	close(output);

	return 0;
}


int execFork(char** argsv)
{
	pid_t pid;

	int returnValue;

	pid = fork();

	if (pid < 0)
	{
		perror("Unable to fork");

		exit(-1);
	}

	if (pid == 0)
	{
		execvp(argsv[0], argsv);

		exit(-1);
	}

	wait(&returnValue);

	return WEXITSTATUS(returnValue);
}

int wirteResult(char* name, char* result)
{
	char stuRes[MAX_PATH] = "";

	int strSize, resFile = open("result.csv", O_WRONLY | O_APPEND);

	if (resFile < 0)
	{
		perror("Unable to open result.csv");

		exit(-1);
	}

	appendS(stuRes, name);

	appendS(stuRes, "  ");

	appendS(stuRes, result);

	appendS(stuRes, "\n");

	strSize = stringLen(stuRes);

	if (write(resFile, stuRes, strSize) < 0)
	{
		perror("Unable to wirte to result.csv");

		exit(-1);
	}

	return 0;
}


int stringLen(char* str)
{

	int i = 0;

	while (str[i] != '\0')
		i++;

	return i;
}


