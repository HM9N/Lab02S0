#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "pathModule.h"
#include "textTools.h"

#define MAX_SIZE 100
#define MAX_SIZE_SEARCH_PATH 300
#define error_message "An error has occurred\n"

/************************************************************************************************* 
  **********************************************************************************************
Código realizado por Jhon Vásquez para el curso de Sistemas Operativos de la Universidad de Antioquia 
  **********************************************************************************************
**************************************************************************************************/

int main(int argc, char *argv[])
{
    char *str = (char *)malloc(sizeof(char) * MAX_SIZE);
    char *searchPath[MAX_SIZE_SEARCH_PATH];
    char *arr[MAX_SIZE], *pathArr[MAX_SIZE];
    searchPath[0] = "/bin";
    searchPath[1] = NULL;

    do
    {
        if (argc == 1)
        {
            printf("wish> ");
            fgets(str, MAX_SIZE, stdin);
            replaceLineBreak(&str);
        }
        else if (argc > 2)
        {
            printf("Error");
            exit(0);
        }

        char *strAux = (char *)malloc(sizeof(char) * MAX_SIZE);
        eliminateCharacters(str);
        strcpy(strAux, str);
        int windex = 0;
        while ((arr[windex] = strsep(&strAux, " \t\a\n\r")) != NULL)
        {
            windex++;
        }

        arr[windex] = NULL;
        free(strAux);

    } while (strcmp(str, "exit\n"));

    return 0;
}