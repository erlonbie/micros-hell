#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "meu_cd.h"

int tam_input = 0;

char **parser(char *input)
{
    char **vetor = (char **)malloc(sizeof(char *) * strlen(input));
    char *token, *parou;
    int i = 0;

    parou = strdup(input);

    while ((token = strsep(&parou, " ")) != NULL)
    {
        vetor[i] = token;
        /* printf("%s\n", vetor[i]); */
        i++;
    }

    tam_input = i;

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

int pos_output (char ** args) {
    int pos_operator = 0;
    for (int i = 0; i < tam_input; ++i) {
        if( strchr(args[i], '>') != NULL) {
            pos_operator = i;
        }
    }

    return pos_operator;
}


int pos_input (char ** args) {
    int pos_operator = 0;
    for (int i = 0; i < tam_input; ++i) {
        if( strchr(args[i], '<') != NULL) {
            pos_operator = i;
        }
    }

    return pos_operator;
}


void cmd_execute (char ** input) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork falhou\n");
        exit(1);
    } else if (rc == 0) {
        if(pos_output(input) > 0) {
            close(STDOUT_FILENO);
            fopen(input[pos_output(input)+1], "w");
            input[pos_output(input)] = NULL;
            execvp(input[0], input);
        } else if (pos_input(input) > 0) {
            close(STDIN_FILENO);
            fopen(input[pos_output(input)+1], "r");
            input[pos_output(input)] = NULL;
            execvp(input[0], input);
        }
        else {
            execvp(input[0], input);
        }
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
        pos_output(args);
        pos_input(args);

        if (strcmp(input, "exit") == 0)
        {
            cmd_exit(); 
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            mudaDiretorio(args[1]);
        }
        else {
            cmd_execute(args); // echo "teste" > a.txt
        }

        free(input);
        free(args);
    }

    return 0;
}
