#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define DELIM " \t\n"
#define PROMPT "$ "
#define MAX_ARGS 30

extern char **environ;

void print_environment(void);
char **tokenizer(char *line, char *delim);
int execute(char **args);
int main(void);

#endif
