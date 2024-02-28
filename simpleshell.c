#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 20

/**
 * tokenizer - converts a string into tokens.
 * @lineptr: the string to be tokenized.
 * Return: returns an array of converted tokens.
 */
char **tokenizer(char *lineptr)
{
	char **args = malloc(MAX_ARGS * sizeof(char *));

	int i = 0;

	args[i] = strtok(lineptr, " ");

	while (args[i] != NULL && i < MAX_ARGS - 1)
	{
		args[++i] = strtok(NULL, " ");
	}
	args[i] = NULL;

	return (args);
}

/**
 * execute_command - executes the passed command.
 * @args: the commands to be executed.
 * Return: Nothing.
 */
void execute_command(char **args)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("Fork Failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		execve(args[0], args, NULL);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
			printf("Command not found: %s\n", args[0]);
	}
}

/**
 * main - main shell loop.
 *
 * Return: Always 0.
 */
int main(void)
{
	char *lineptr = NULL;
	size_t len = 0;
	ssize_t read;
	char **args;
	int str_count;

	while (1)
	{
		printf("$ ");
		read = getline(&lineptr, &len, stdin);

		if (read == -1 || strcmp(lineptr, "exit\n") == 0)
		{
			printf("\n");
			break;
		}

		str_count = strlen(lineptr);

		lineptr[str_count - 1] = '\0';

		args = tokenizer(lineptr);

		execute_command(args);
	}
	free(lineptr);
	return (0);
}

