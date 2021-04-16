#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "meu_cd.h"

void mudaDiretorio(char *path)
{
    struct passwd *nomeDir = getpwuid(getuid());
    char *home = nomeDir->pw_dir;
    int valor = -1;
    if (path == NULL)
    {
        valor = chdir(home);
    }
    else
    {
        valor = chdir(path);
    }
    if (valor == -1)
    {
        printf("Erro\n");
    }
}