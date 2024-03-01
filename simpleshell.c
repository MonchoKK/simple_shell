#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define DELIM " \t\n"
#define PROMPT "$ "
#define MAX_ARGS 30

/**
 * print_environment - function printing the environment
 *
 * Return: Always 0.
 */
void print_environment(void)
{
	extern char **environ;
	char **env = environ;

	while (*env)
	{
		printf("%s\n", *env++);
	}
}

/**
 * tokenizer - converts a string into tokens.
 * @line: the string to be tokenized.
 * @delim: delimiter to separate strings.
 * Return: returns an array of converted tokens.
 */
char **tokenizer(char *line, char *delim)
{
	int i = 0;
	char **args = malloc((MAX_ARGS + 1) * sizeof(char *));

	if (args == NULL)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	args[i] = strtok(line, delim);

	while (args[i] != NULL)
	{
		args[++i] = strtok(NULL, delim);
	}
	args[i] = NULL;

	return (args);
}

/**
 * execute - uses execve to execute command
 * @args: array of arguments to be executed
 *
 * Return: 1 on success, 0 on exit and -1 on error
 */
int execute(char **args)
{
	pid_t pid;
	int status;

	if (strcmp(args[0], "exit") == 0)
	{
		return (0);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		print_environment();
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		return (-1);
	}
	if (pid == 0)
	{
		if (execve(args[0], args, NULL) == -1)
		{
			perror(args[0]);
			exit(1);
		}
	}
	else
	{
		wait(&status);
	}
	return (1);
}

/**
 * main - a simple shell loop
 *
 * Return: 0 on success, 1 on error
 */
int main(void)
{
	char *line, **args;
	int status;
	size_t len = 0;

	line = NULL;

	while (1)
	{
		printf(PROMPT);
		if (getline(&line, &len, stdin) == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			else
			{
				perror("getline");
				return (1);
			}
		}
		args = tokenizer(line, DELIM);
		status = execute(args);

		if (status == 0)
			break;
		if (status == -1)
			return (1);
	}
	free(line);
	free(args);
	return (0);
}
