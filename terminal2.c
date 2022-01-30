#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "meu_ls.h"
#include "meu_cd.h"
#include "meu_exec.h"

void prompt()
{
    printf(">> ");
}

char **parser(char *frase)
{
    char **vetor = (char **)malloc(sizeof(char *) * strlen(frase));
    char *token, *parou;
    int i = 0;

    parou = strdup(frase);

    while ((token = strsep(&parou, " ")) != NULL)
    {
        vetor[i] = token;
        i++;
    }

    return vetor;
}

int main(int argc, char *argv[])
{
    system("clear");
    int tam;
    char **args;
    while (1)
    {
        //prompt();
        char *path = getcwd(NULL, 0);
        printf("%s >> ", path);

        char frase[100];
        if (argc > 1)
        {
            args = argv;
        }
        else
        {
            scanf("%[^\n]", frase);
            getchar();
            args = parser(frase);
        }

        for (int j = 0; args[j] != NULL; j++)
        {
            tam = j;
            //printf("Token %d: %s\n", j, args[j]);
        }
        tam++;
        if (tam == 1)
        {
            args[1] = "";
        }
        //printf("tamanho: %d\n", tam);
        if (strcmp(frase, "exit") == 0)
        {
            int pid = getpid();
            char str[5] = {0};
            sprintf(str, "%d", pid);
            args[0] = strdup("kill");
            args[1] = strdup(str);
            args[2] = NULL;
            execvp((args[0]), args);
        }
        else if (strcmp(args[0], "ls") == 0)
        {
            mostraArquivos(path);
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            printf("%s\n", path);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            mudaDiretorio(args[1]);
        }
        else
        {
            executaComando(args, tam);
        }
    }

    return 0;
}
