#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "pathModule.h"

void modifySearchPath(char *searchPath[])
{
}

int isPath(char *path)
{
    //printf("hola%s d", path) ;
    if (!access(path, X_OK))
    {
        return 1;
    }
    return 0;
}

int searchPaths(char **path, char *args[], int pathCounter, int *pathPosition)
{
    char *bar = "/";
    int exists = 0;
    int i = 0;
    char pathAndFile[100];
    while (i < pathCounter)
    {
        strcpy(pathAndFile, path[i]);
        strcat(pathAndFile, bar);
        strcat(pathAndFile, args[0]);
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

void executeCommand(char *path, char *args[], int isRed)
{
    printf("HOLA desde ExecuteCommand");
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("soy el proceso hijo %d ", getpid());
        char aux[100];
        char *bar = "/";
        strcpy(aux, path);
        int i = 0;
        char *const *arguments = args;
        strcat(aux, bar);
        strcat(aux, arguments[0]);
        int error;

        if (isRed == 1)
        {
            printf("Es redirección");
            int i = 0;
            char *argumentsRedirection[sizeof(arguments)];
            while (arguments[i] != "<")
            {
                argumentsRedirection[i] = arguments[i];
                i++;
            }
            argumentsRedirection[i] = NULL;
            printf("%s \n", aux);
            error = execv(aux, argumentsRedirection);
        }
        else
        {
            printf("%d \n", isRed);
            printf("%d \n", isRed);
            error = execv(aux, arguments);
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