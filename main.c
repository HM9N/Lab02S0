#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtinCommand.h"
#include "pathModule.h"

#define MAX_SIZE 100
#define MAX_SIZE_SEARCH_PATH 300

/************************************************************************************************* 
  **********************************************************************************************
Código realizado por Jhon Vásquez para el curso de Sistemas Operativos de la Universidad de Antioquia 
  **********************************************************************************************
**************************************************************************************************/

int main(int argc, char *argv[])
{
    char *searchPath[MAX_SIZE_SEARCH_PATH];
    searchPath[0] = "/bin";
    char commandSearched[100];
    strcpy(commandSearched, searchPath[0]);
    char str[MAX_SIZE];
    int pathCounter = 1;

    do
    {
        printf("wish> ");
        if (argc == 1)
        {
            fgets(str, MAX_SIZE, stdin);
        }
        char *arr[] = {str, NULL};
        char *p = str;
        while (*p != '\n')
        {
            p++;
        }
        *p = '\0';
        builtinCommand command = strToCommand(str);
        if (command != not_command)
        {
            char *aux = "/bin/ls";
            char *arguments[] = {"ls", NULL};
            switch (command)
            {
            case cd:
                execv(aux, arguments);
                break;
            case path:
                printf("path Executed\n");
                break;
            case endup:
                exit(0);
                break;
            default:
                printf("Comando no encontrado\n");
            }
        }
        else
        {
            int pathPosition = 0;
            // Busca el ejecutable en las rutas, si ejecuta el programa devuelve un uno, en caso contrario devuelve un 0
            int existPaths = searchPaths(searchPath, arr, pathCounter, &pathPosition);
            if (existPaths == 1)
            {
                executeCommand(searchPath[pathPosition], arr);
            }
            else
            {
                printf("No existe el comando \n");
            }
        }
    } while (strcmp(str, "exit\n"));

    return 0;
}