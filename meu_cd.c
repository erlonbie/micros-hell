#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "meu_cd.h"

char *callwd()
{
    return getcwd(NULL, 0);
}
void setwd(char *path)
{
    struct passwd *pw = getpwuid(getuid());
    const char *home = pw->pw_dir;
    int result = -1;
    if (path == NULL)
    {
        result = chdir(home);
    }
    else
    {
        result = chdir(path);
    }
    if (result == -1)
    {
        switch (errno)
        {
        case ENOENT:
            printf("Não foi possível encontrar: %s", path);
            break;
        case EACCES:
            printf("Não há permissão para acessar: %s", path);
            break;
        default:
            break;
        }
    }
}