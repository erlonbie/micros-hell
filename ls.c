#include <errno.h> // disonibbiliza a variavel errno que indica o tipo de erro
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "ls.h"

void mostraArquivos(char *path)
{

    DIR *dp;
    dp = opendir(path);

    struct dirent *dirp = readdir(dp);
    char *dirpname;

    errno = 0;

    while (dirp)
    {
        dirpname = dirp->d_name;
        if (dirpname[0] != '.')
        {
            printf("%s\n", dirpname);
        }
        dirp = readdir(dp);
    }

    if (errno != 0)
    { // tratamentos se teve erro
        switch (errno)
        {
        case ENOENT:
            printf("Esse diretorio não existe%s", path);
            break;
        default:
            break;
        }
    }

    // closedir(dp);
}
