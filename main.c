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
    printf("%d", argc);
    char *searchPath[MAX_SIZE_SEARCH_PATH];
    searchPath[0] = "/bin/";
    char commandSearched[100];
    strcpy(commandSearched, searchPath[0]);
    char str[MAX_SIZE];

    do
    {
        printf("wish> ");
        if (argc == 1)
        {
            fgets(str, MAX_SIZE, stdin);
        }
        char *arr[] = {"ls", NULL};
        char *p = str;
        while (*p != '\n')
        {
            p++;
        }
        *p = '\0';
        builtinCommand command = strToCommand(str);
        if (command != not_command)
        {
            switch (command)
            {
            case cd:
                /*  strcat(commandSearched, str);
                printf("%s", commandSearched);
                execv(commandSearched, arr); */
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
            // printf("Buscar el comando en la ruta path\n");
            int existPaths = existPath(searchPath, str);
        }
    } while (strcmp(str, "exit\n"));

    return 0;
}