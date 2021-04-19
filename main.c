#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            case jason:
                printf("Jason Executed\n");
                break;
            case abril:
                printf("Abril Executed\n");
                break;
            case diana:
                printf("Diana Executed\n");
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