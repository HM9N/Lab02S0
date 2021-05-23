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
    char *arr[MAX_SIZE], *pathArr[MAX_SIZE], *arrRed[MAX_SIZE], *str = (char *)malloc(sizeof(char) * MAX_SIZE), *searchPath[MAX_SIZE_SEARCH_PATH], commandSearched[100], redirectionFile[100];
    searchPath[0] = "/bin";
    searchPath[1] = NULL;
    strcpy(commandSearched, searchPath[0]);
    int isRed, pathIndex = 0, pathCounter, pathModified = 0, line = 0, countRed;
    size_t numero_bytes = MAX_SIZE;
    FILE *file;

    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
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
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
        else if (argc == 2 && strcmp(argv[0], "wish") != 0)
        {
            line = getline(&str, &numero_bytes, file);

            countRed = 0;
            for (int i = 0; i < strlen(str); i++)
            {
                //printf("El caracter es: %d\n", str[i]);
                if (str[i] != 32 && str[i] != '\t' && str[i] != '\a' && str[i] != 10)
                {
                    countRed++;
                }
            }

            if (countRed == 0)
            {
                //printf("Voy a continuar\n");
                continue;
            }

            if (line == -1)
            {
                if (argc != 1 && feof(file))
                {
                    fclose(file);
                    exit(0);
                }
            }
            replaceLineBreak(&str);
        }

        char *strAux = (char *)malloc(sizeof(char) * MAX_SIZE);
        eliminateCharacters(str);
        strcpy(strAux, str);
        //printf("Voy a ejecutar el comando %s \n", str);

        int windex = 0;
        pathIndex = 0;
        while ((arr[windex] = strsep(&strAux, " \t\a\n\r")) != NULL)
        {
            if ((windex >= 1) && (!strcmp(arr[0], "path") || !strcmp(arr[0], "cd") || !strcmp(arr[0], "exit")))
            {
                pathArr[pathIndex] = arr[windex];
                pathIndex++;
            }

            windex++;
        }

        arr[windex] = NULL;
        free(strAux);

        isRed = isRedirection(str);

        if (isRed)
        {
            countRed = 0;
            char *strAuxRed = (char *)malloc(sizeof(char) * MAX_SIZE);
            strcpy(strAuxRed, str);
            /* printf("El valor longitud es:%ld \n", strlen(strAuxRed)); */

            int k = 0;

            while ((arrRed[k] = strsep(&strAuxRed, ">")) != NULL)
            {
                k++;
            }

            arrRed[k] == NULL;
            char *auxToEliminateChars = (char *)malloc(sizeof(char) * MAX_SIZE);
            strcpy(auxToEliminateChars, arrRed[1]);
            eliminateCharacters(auxToEliminateChars);
            if (strstr(auxToEliminateChars, " ") == NULL && k == 2)
            {
                strcpy(redirectionFile, arrRed[1]);
                for (int i = 0; i < strlen(arrRed[1]); i++)
                {
                    if (arrRed[1][i] != ' ' && arrRed[1][i] != '\t' && arrRed[1][i] != '\a' && arrRed[1][i] != '\n')
                    {
                        countRed++;
                    }
                }
            }
            else
            {
                strcpy(redirectionFile, " ");
            }

            eliminateCharacters(redirectionFile);
            free(strAuxRed);
            free(auxToEliminateChars);
        }

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
                executeCommand(searchPath[pathPosition], arr, isRed, countRed, redirectionFile);
            }
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
            }
        }

    } while (strcmp(str, "exit\n"));

    return 0;
}