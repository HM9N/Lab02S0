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
    char *arr[MAX_SIZE], *pathArr[MAX_SIZE], *arrRed[MAX_SIZE], *str = (char *)malloc(sizeof(char) * MAX_SIZE), *searchPath[MAX_SIZE_SEARCH_PATH];
    char *arrParCom[MAX_SIZE], commandSearched[100], redirectionFile[100];
    searchPath[0] = "/bin";
    searchPath[1] = NULL;
    strcpy(commandSearched, searchPath[0]);
    int isRed, pathIndex = 0, pathCounter = 0, pathModified = 0, line = 0, countRed, countArguments, isParCom, parComCounter;
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
                if (str[i] != 32 && str[i] != '\t' && str[i] != '\a' && str[i] != 10)
                {
                    countRed++;
                }
            }

            if (countRed == 0)
            {
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

        eliminateCharacters(str);

        //Inicio For

        isParCom = isParallelCommand(str);

        // Verifica si son comandos paralelos
        if (isParCom)
        {
            char *strAuxParCom = (char *)malloc(sizeof(char) * MAX_SIZE);

            parComCounter = formatParallelCommand(str, strAuxParCom, arrParCom);
            //printf("Es paralelo\n");
            if (parComCounter == -1)
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                continue;
            }
        }
        else
        {
            arrParCom[0] = str;
            parComCounter = 1;
        }

        //printf("El parComcounter es: %d\n", parComCounter);

        // for que se repite dependiendo la cantidad de comandos paralelos
        for (int l = 0; l < parComCounter; l++)
        {
            char *strAux = (char *)malloc(sizeof(char) * MAX_SIZE);
            /* eliminateCharacters(str); */
            //strcpy(strAux, str);
            strcpy(strAux, arrParCom[l]);
            //printf("Voy a ejecutar el comando %s \n", str);

            if (strlen(strAux) == 0)
            {
                continue;
            }

            int windex = 0;
            countArguments = 0;
            while ((arr[windex] = strsep(&strAux, " \t\a\n\r")) != NULL)
            {
                if ((windex >= 1) && !strcmp(arr[0], "path"))
                {
                    pathArr[pathIndex] = arr[windex];
                    pathIndex++;
                }
                else if (windex == 0 && !strcmp(arr[0], "path"))
                {
                    pathIndex = 0;
                }

                if ((windex >= 1) && (!strcmp(arr[0], "cd") || !strcmp(arr[0], "exit")))
                {
                    pathArr[countArguments] = arr[windex];
                    countArguments++;
                }

                windex++;
            }

            arr[windex] = NULL;
            free(strAux);

            //isRed = isRedirection(str);
            isRed = isRedirection(arrParCom[l]);

            if (isRed)
            {
                countRed = 0;
                char *strAuxRed = (char *)malloc(sizeof(char) * MAX_SIZE);
                //strcpy(strAuxRed, str);
                strcpy(strAuxRed, arrParCom[l]);
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
                    if (countArguments != 1 || chdir(pathArr[0]) != 0)
                    {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    }
                    break;
                case path:
                    pathModified = 1;
                    modifySearchPath(searchPath, pathArr, &pathIndex);
                    //pathModifiedNow = 1;
                    break;
                case endup:
                    if (countArguments > 0)
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
                //printf("No recuerdo esas cosas de if\n");
                int pathPosition = 0; // Variable para devolver la posición del path en el search path
                                      // Busca el ejecutable en las rutas, si ejecuta el programa devuelve un uno, en caso contrario devuelve un 0
                                      /* if (isRed && pathModified == 1 && pathModifiedNow ==1)
                {
                    pathCounter = pathIndex - 2;
                } */
                if ((pathIndex != 0) && pathModified == 1)
                {
                    pathCounter = pathIndex;
                }
                else if (pathModified == 0)
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

               // printf("El pathCounter es: %d\n", pathCounter);
            }
        }

        // END FOR

    } while (strcmp(str, "exit\n"));

    return 0;
}