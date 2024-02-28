#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define X_OK 1
#define F_OK 0
#define MAX_ARGS 20

extern char **env;

int main(void) {
    char *lineptr = NULL;
    size_t len = 0;
    ssize_t read;
    pid_t pid;
    int status;

    while (1) {
        printf("$ ");
        read = getline(&lineptr, &len, stdin);

        if (read == -1 || strcmp(lineptr, "exit\n") == 0) {
            printf("\n");
            break;
        }

        int str_count = strlen(lineptr);
        lineptr[str_count - 1] = '\0';

        char *argv[MAX_ARGS];
        int i = 0;

        argv[i] = strtok(lineptr, " ");

        while (argv[i] != NULL && i < MAX_ARGS - 1) {
            argv[++i] = strtok(NULL, " ");
        }
        argv[i] = NULL;

        if (access(argv[0], X_OK) == 0 && !access(argv[0], F_OK)) {
            pid = fork();
            if (pid == -1) {
                perror("Fork Failed");
                exit(EXIT_FAILURE);
            }
            if (pid == 0) {
                execve(argv[0], argv, env);
            } else {
                waitpid(pid, &status, 0);

                if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                    printf("Command not found: %s\n", argv[0]);
                }
            }
        } else {
            printf("Command not found or not executable: %s\n", argv[0]);
        }
    }

    free(lineptr);
    return 0;
}
