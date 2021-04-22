#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtinCommand.h"

#define MAX_SIZE 100

/************************************************************************************************* 
  **********************************************************************************************
Código realizado por Jhon Vásquez para el curso de Sistemas Operativos de la Universidad de Antioquia 
  **********************************************************************************************
**************************************************************************************************/

int main(int argc, char *argv[])
{
    char str[MAX_SIZE];
    do
    {
        printf("wish> ");
        fgets(str, MAX_SIZE, stdin);
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
                /* printf("cd Executed\n"); */
                printf("Hello ");
                printf("ya ejecuté");
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
            printf("Buscar el comando en la ruta path\n");
        }
    } while (strcmp(str, "exit\n"));
}