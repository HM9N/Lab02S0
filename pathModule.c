#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "pathModule.h"

// Se da la opción de modificar el Search Path al ejecutar el comando integrado "path"
void modifySearchPath(char *searchPath[], char *args[], int *pathCounter)
{
    int i = 0;
    char *charUsedToDelete = "\0";
    // printf("El pathCounter es %d\n", *pathCounter);

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
    printf("número pathCounter: %d \n", pathCounter);
    printf("%s path \n", path[0]);
    char *bar = "/";
    int exists = 0;
    int i = 0;
    char pathAndFile[100];
    while (i < pathCounter)
    {
        strcpy(pathAndFile, path[i]);
        strcat(pathAndFile, bar);
        strcat(pathAndFile, args[0]);
        printf("El pathAndFile es %s \n", pathAndFile);
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
void executeCommand(char *path, char *args[], int isRed)
{
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        char aux[100];
        char *bar = "/";
        strcpy(aux, path);
        int i = 0;
        strcat(aux, bar);
        strcat(aux, args[0]);
        int error;
        if (isRed == 1)
        {
            unlink("output.txt");
            int redirect_fd = open("output.txt", O_CREAT | O_TRUNC | O_WRONLY | O_EXCL, S_IRWXU);
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
            error = execv(aux, args);
        }

        if (error == -1)
        {
            printf("ha sucedido un error");
        }
    }
    else
    {
        int rc_wait = wait(NULL);
    }
}