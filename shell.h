#ifndef SHELL_H
#define SHELL_H

extern char **environ;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <erno.h>
#include <signal.h>

#define DELIM " \t\n"
#define PROMPT "$ "
#define MAX_ARGS 30

void print_environment(void);
char **tokenizer(char *line, char *delim);
int execute(char **args);

#endif
