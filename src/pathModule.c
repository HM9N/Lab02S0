#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "pathModule.h"
#define error_message "An error has occurred\n"

// Se da la opción de modificar el Search Path al ejecutar el comando integrado "path"
void modifySearchPath(char *searchPath[], char *args[], int *pathCounter)
{

    int i = 0;
    char *charUsedToDelete = "\0";

    //printf("El pathCounter es: %d\n", *pathCounter);

    if (*pathCounter == 0)
    {
        while (searchPath[i] != NULL)
        {
            searchPath[i] = "\0";
            i++;
        }
    }
    else
    {
        while (i < *pathCounter)
        {
            searchPath[i] = args[i];
            i++;
        }
        searchPath[i] = NULL;
    }
    int k = 0;
    /*  while (searchPath[k] != NULL)
    {
        printf("El path %d es: %s\n", k, searchPath[k]);
        k++;
    } */
}

//Verifica que exista el ejecutable en la ruta especificada
int isPath(char *path)
{
    if (!access(path, X_OK))
    {
        return 1;
    }
    return 0;
}

//Organiza los argumentos y la ruta para buscar si existe el ejecutable
int searchPaths(char **path, char *args[], int pathCounter, int *pathPosition)
{
    //printf("El puto pathcounter mariquiando otra vez %d\n", pathCounter);
    char *bar = "/";
    int exists = 0;
    int i = 0;
    char pathAndFile[100];
    while (i < pathCounter && exists == 0)
    {
        strcpy(pathAndFile, path[i]);
        strcat(pathAndFile, bar);
        strcat(pathAndFile, args[0]);
        //printf("El pathAndFile es %s \n", pathAndFile);
        exists = isPath(pathAndFile);
        i++;
    }

    *pathPosition = i - 1;
    if (exists == 1)
    {
        return 1;
    }
    return 0;
}

// Ejecuta el comando
void executeCommand(char *path, char *args[], int isRed, int countRed, char *redirectionFile, int isParCom, int *forks, int *countforks, int parComCounter)
{
    int rc = fork();
    forks[*countforks] = rc;
    forks[*countforks + 1] = 777;

    if (rc < 0)
    {
        write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }
    else if (rc == 0)
    {
        char aux[100];
        char *bar = "/";
        strcpy(aux, path);
        strcat(aux, bar);
        strcat(aux, args[0]);
        int error = 0;
        if (isRed == 1)
        {
            if (countRed != 0)
            {
                unlink(redirectionFile);
                int redirect_fd = open(redirectionFile, O_CREAT | O_TRUNC | O_WRONLY | O_EXCL, S_IRWXU);
                dup2(redirect_fd, STDOUT_FILENO);
                int i = 0;
                char *argumentsRedirection[sizeof(*args)];
                while (*args[i] != '>')
                {
                    argumentsRedirection[i] = args[i];
                    i++;
                }
                argumentsRedirection[i] = NULL;
                error = execv(aux, argumentsRedirection);
            }
            else
            {
                write(STDERR_FILENO, error_message, strlen(error_message));
                exit(1);
            }
        }
        else
        {
            error = execv(aux, args);
        }

        if (error == -1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }
    }
    else
    {
        int status;

        if (isParCom && parComCounter - 1 == *countforks)
        {
            int hola;

            for (int f = 0; forks[f] != 777; f++)
            {
                // printf("El proceso que va a esperar es:%d\n", forks[f]);
                waitpid(forks[f], &status, WUNTRACED);
            }
        }

        if (!isParCom)
        {
            int rc_wait = wait(NULL);
        }
    }
}