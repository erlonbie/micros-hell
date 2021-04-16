#include <errno.h> // disonibbiliza a variavel errno que indica o tipo de erro
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include "meu_ls.h"

void mostraArquivos(char *path)
{
	DIR *diretorio;
	diretorio = opendir(path);
	struct dirent *pathDiretorio = readdir(diretorio);
	char *nomeDiretorio;
	while (pathDiretorio)
	{
		nomeDiretorio = pathDiretorio->d_name;
		if (nomeDiretorio[0] != '.')
		{
			printf("%s  ", nomeDiretorio);
		}
		pathDiretorio = readdir(diretorio);
	}
	printf("\n");
}
