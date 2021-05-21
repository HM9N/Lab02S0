#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "builtinCommand.h"
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
    char *searchPath[MAX_SIZE_SEARCH_PATH];
    searchPath[0] = "/bin";
    searchPath[1] = NULL;
    char commandSearched[100];
    strcpy(commandSearched, searchPath[0]);
    char *str = (char *)malloc(sizeof(char) * MAX_SIZE);
    char *arr[MAX_SIZE], *pathArr[MAX_SIZE];
    int isRed, pathIndex = 0, pathCounter, pathModified = 0;
    size_t numero_bytes = MAX_SIZE;
    int line = 0;
    FILE *file;

    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(0);
        }
    }

    do
    {
        if (argc == 1)
        {
            printf("wish> ");
            fgets(str, MAX_SIZE, stdin);
        }
        else if (argc > 2)
        {
            printf("Error");
            exit(0);
        }
        else if (argc == 2 && strcmp(argv[0], "wish") != 0)
        {
            line = getline(&str, &numero_bytes, file);

            if (line == -1)
            {
                if (argc != 1 && feof(file))
                {
                    fclose(file);
                    exit(0);
                }
            }
            //fgets(str, 100, file);

            replaceLineBreak(&str);

            // printf("El tamaño es: %ld\n", strlen(str));

            for (int i = 0; i < strlen(str); i++)
            {
                //printf("El código ASCII es: %d\n", str[i]);
                //printf("El apuntador es: %p\n", &str);
                /* if (str[i] == EOF)
                {
                    printf("Llegamos al final \n ");
                } */
            }
        }

        char *strAux = (char *)malloc(sizeof(char) * MAX_SIZE);
        eliminateCharacters(str);
        strcpy(strAux, str);
        //printf("Voy a ejecutar el comando %s \n", str);
        int windex = 0;
        while ((arr[windex] = strsep(&strAux, " \t\a\n\r")) != NULL)
        {
            if ((windex == 0))
            {
                pathIndex = 0;
            }

            if ((windex >= 1) && !strcmp(arr[0], "path") || (windex >= 1) && !strcmp(arr[0], "cd") || (windex >= 1) && !strcmp(arr[0], "exit"))
            {
                pathArr[pathIndex] = arr[windex];
                pathIndex++;
            }

            windex++;
        }

        arr[windex] = NULL;
        free(strAux);

        isRed = isRedirection(str);

        builtinCommand command = strToCommand(arr[0]);
        if (command != not_command)
        {
            char s[100];
            switch (command)
            {
            case cd:
                if (pathIndex != 1 || chdir(pathArr[0]) != 0)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                }
                break;
            case path:
                pathModified = 1;
                modifySearchPath(searchPath, pathArr, &pathIndex);
                break;
            case endup:
                if (pathIndex > 0)
                {
                    write(STDERR_FILENO, error_message, strlen(error_message));
                    break;
                }
                else
                {
                    exit(0);
                }
                break;
            default:
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }
        else
        {
            int pathPosition = 0; // Variable para devolver la posición del path en el search path
            // Busca el ejecutable en las rutas, si ejecuta el programa devuelve un uno, en caso contrario devuelve un 0
            if (isRed && pathModified == 1)
            {
                pathCounter = pathIndex - 2;
            }
            else if ((pathIndex != 0) && pathModified == 1)
            {
                pathCounter = pathIndex;
            }
            else
            {
                pathCounter = 1;
            }

            int existPaths = searchPaths(searchPath, arr, pathCounter, &pathPosition);

            if (existPaths == 1)
            {
                //Función para ejecutar un comando externo
                executeCommand(searchPath[pathPosition], arr, isRed);
            }
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }

    } while (strcmp(str, "exit\n"));

    return 0;
}