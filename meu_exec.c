#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "meu_exec.h"

void executaComando(char **args, int tam)
{
    int pipefd[2] = {-1, -1};
    int rc = fork();
    if (rc < 0)
    {
        printf("Erro!\n");
        return;
    }
    if (rc == 0)
    {
        if (tam > 1)
        {
            pipe(pipefd);
            int rc2 = fork();
            if (rc2 < 0)
            {
                printf("Erro!\n");
                return;
            }
            else if (rc2 == 0)
            {
                char **argsFilho = args;
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[1]);
                //printf("PASSEI POR AQUI\n");

                int status2 = execvp(argsFilho[0], argsFilho);
                printf("\n");

                if (status2 < 0)
                {
                    printf("Erro!\n");
                    exit(1);
                }
            }
            else
            {
                dup2(pipefd[1], 1);
                int status = execvp(args[0], args);
                printf("\n");
                if (status < 0)
                {
                    printf("Erro!\n");
                    exit(1);
                }
                wait(0);
            }
        }
        else
        {
            if (args == NULL)
                exit(1);
            //int status =
            execvp(args[0], args);
            printf("\n");
            // if (status < 0)
            // {
            //     printf("Erro ao executar programa\n");
            //     exit(1);
            // }
        }
    }
    else
    {
        if (strcmp(args[1], "&") == 0)
            printf("Processo filho: [%d]\n", rc);
        else
            wait(NULL);
    }
}