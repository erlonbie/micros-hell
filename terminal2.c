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

int pos_token (char ** args, char token) {
    int pos_operator = 0;
    for (int i = 0; i < tam_input; ++i) {
        if( strchr(args[i], token) != NULL) {
            pos_operator = i;
        }
    }

    return pos_operator;
}

void print_strings(char ** strs) {
    for (int i = 0; strs[i]; i++) printf("%d:'%s' \n", i, strs[i]);
}

int tam_arq(FILE *arq) {
    int pos_atual = ftell(arq);
    fseek(arq, 0, SEEK_END);
    int tam = ftell(arq);
    fseek(arq, pos_atual, SEEK_SET);
    return tam;
}

void cmd_execute (char ** input) {
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork falhou\n");
        exit(1);
    } else if (rc == 0) {
        int token_saida = pos_token(input, '>');
        int token_entrada = pos_token(input, '<');
        if(token_saida > 0) {
            close(STDOUT_FILENO);
            fopen(input[token_saida+1], "w");
            input[token_saida] = NULL;
            execvp(input[0], input);
        } else if (token_entrada > 0) {
            FILE *f = fopen(input[token_entrada+1], "r");
            if (f) {
                int tam_input = tam_arq(f);
                char *string_arq = (char*) malloc(tam_input);
                fread(string_arq, tam_input, 1, f);
                char **new_args = (char**) malloc(sizeof(char*) * 2);
                new_args[0] = input[0];
                new_args[1] = string_arq;
                execvp(input[0], new_args);
            } else {
                printf("Arquivo '%s' não encontrado.", input[token_entrada+1]);
            }
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
