#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char **parser(char *input)
{
    char **vetor = (char **)malloc(sizeof(char *) * strlen(input));
    char *token, *parou;
    int i = 0;

    parou = strdup(input);

    while ((token = strsep(&parou, " ")) != NULL)
    {
        vetor[i] = token;
        printf("%s\n", vetor[i]);
        i++;
    }

    return vetor;
}

void cmd_exit () {
    int pid = getpid();
    char str[5] = {0};
    char * args[3];
    sprintf(str, "%d", pid);
    args[0] = strdup("kill");
    args[1] = strdup(str);
    args[2] = NULL;
    execvp((args[0]), args);
}

void cmd_execute (char ** input) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork falhou\n");
        exit(1);
    } else if (rc == 0) {
        execvp(input[0], input);
        cmd_exit(); 
    }
    else {
        int cmd = wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    system("clear");
    int tam;
    char **args;
    char * input;
    while (1)
    {
        char *path = getcwd(NULL, 0);
        printf("%s >> ", path);

        /* char input[100]; */
        input = (char *) malloc(sizeof(char));
        scanf("%[^\n]", input);
        getchar();
        args = parser(input);

        if (strcmp(input, "exit") == 0)
        {
            cmd_exit(); 
        }
        else {
            cmd_execute(args);
        }

        free(input);
        free(args);
    }

    return 0;
}
