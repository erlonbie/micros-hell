/*
    Aldemir Silva
    Erlon Bié
    Glenn Fonseca
    Igor Carvalho
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_ARGS 100
#define READ_END 0
#define WRITE_END 1

typedef struct Args_t {
    char *tokens[MAX_ARGS];
    int length;
} Args_t;

void cd (char* path) {
    if(chdir(path) == -1) {
        fprintf(stderr, "Erro ao mudar de diretório\n");
    }
}

Args_t* args_parser (char *input) {
    Args_t *args = (Args_t*)malloc(sizeof(Args_t));
    char *token = strtok(input, " ");
    int i = 0;

    while (token && i < MAX_ARGS) {
        if(strcmp(token, " ")){
            args->tokens[i++] = token;
        }
        token = strtok(NULL, " ");
    }

    args->length = i;

    for(i = args->length; i < MAX_ARGS; i++) args->tokens[i] = NULL;

    return args;
}

int pos_token (Args_t *args, char token) {
    for (int i = 0; i < args->length; i++) {
        if( strchr(args->tokens[i], token) != NULL) {
            return i;
        }
    }
    return -1;
}

int file_size(FILE *f) {
    int pos_atual = ftell(f);
    fseek(f, 0, SEEK_END);
    int tam = ftell(f);
    fseek(f, pos_atual, SEEK_SET);
    return tam;
}

char **cortar_args(char **args, int corte) {
    char **novos_args = (char **) malloc(sizeof(char*)*corte);
    for (int i = 0; i < corte; i++) {
        novos_args[i] = args[i];
    }
    novos_args[corte-1] = NULL;
    return novos_args;
}



void execute (Args_t *args) {
    int rc = fork();
    int is_wait = pos_token(args, '&');
    if (rc < 0) {
        fprintf(stderr, "fork falhou\n");
        exit(1);
    } else if (rc == 0) {//no filho
        int token_saida = pos_token(args, '>'), token_entrada = pos_token(args, '<'), token_pipe = pos_token(args, '|');

        if (token_pipe > 0) {
            pid_t pid;
            int fd[2];

            pipe(fd);
            pid = fork();

            if(pid==0)//filho do filho -> ret5
            {   
                close(fd[READ_END]);
                dup2(fd[WRITE_END],STDOUT_FILENO);
                close(fd[WRITE_END]);
                char **new_args = cortar_args(args->tokens, token_pipe+1);
                execvp(new_args[0], new_args);
            }
            else //filho
            { 
                pid=fork();
                if(pid==0) //filho2 do filho -> sum5
                {
                    dup2(fd[READ_END],STDIN_FILENO);
                    close(fd[READ_END]);
                    close(fd[WRITE_END]);
                    char **new_args = &(args->tokens[token_pipe+1]);
                    execvp(new_args[0], new_args);
                } else {
                    wait(NULL);
                    close(fd[READ_END]);
                    close(fd[WRITE_END]);
                }
            }
        }
        else if (token_saida > 0) {
            close(STDOUT_FILENO);
            fopen(args->tokens[token_saida+1], "w");
            args->tokens[token_saida] = NULL;
            execvp(args->tokens[0], args->tokens);

        } else if (token_entrada > 0) {
            FILE *f = fopen(args->tokens[token_entrada+1], "r");
            if (f) {
                int size = file_size(f);
                char buffer[size];

                fread(buffer, size, 1, f);

                char *new_args[3] = {args->tokens[0], buffer, (char *)NULL};
                
                execvp(new_args[0], new_args);

            } else {
                printf("Arquivo '%s' não encontrado.", args->tokens[token_entrada+1]);
            }
        } else {
            if (execvp(args->tokens[0], args->tokens) == -1) {
                char aux[strlen(args->tokens[0])+2];
                strcpy(aux, args->tokens[0]);
                strcat(strcpy(args->tokens[0], "./"), aux);

                if(execvp(args->tokens[0], args->tokens) == -1){
                   fprintf(stderr, "Comando não reconhecido\n"); 
                }
            }
        }
        exit(1); 

    }
    if (is_wait < 0) wait(NULL);
}

void interpret(char* input) {
    Args_t *args = args_parser(input);

    // printf("Args Tokens:\n");
    // for(int i = 0; i < args->length; i++){
    //     printf("%s\n", args->tokens[i]);
    // }

    if (strcmp(args->tokens[0], "exit") == 0) {
        exit(1); 
    } else if (strcmp(args->tokens[0], "cd") == 0) {
        cd(args->tokens[1]);
    } else {
        execute(args); // echo "teste" > a.txt
    }

    free(args);
}

int main (int argc, char *argv[]) {
    char *input;

    system("clear");

    while (1) {
        input = (char*)malloc(sizeof(char));

        printf("%s >> ", getcwd(NULL, 0));
        scanf("%[^\n]", input); getchar();

        if(strcmp(input, "") != 0) interpret(input);

        free(input);
    }

    return 0;
}
