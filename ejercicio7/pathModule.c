#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "pathModule.h"
#define error_message "An error has occurred\n"

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
    char *bar = "/";
    int exists = 0;
    int i = 0;
    char pathAndFile[100];
    while (i < pathCounter && exists == 0)
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
void executeCommand(char *path, char *args[])
{
    struct timeval inicio, fin;
    gettimeofday(&inicio, NULL); 
    int rc = fork();
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
        int i = 0;
        strcat(aux, bar);
        strcat(aux, args[0]);
        int error;
  
        error = execv(aux, args);

        if (error == -1)
        {
            write(STDERR_FILENO, error_message, strlen(error_message));
        }
    }
    else
    {
        int rc_wait = wait(NULL);
         gettimeofday(&fin, NULL);
        double secs = (double)(fin.tv_usec - inicio.tv_usec) / 1000000 + (double)(fin.tv_sec - inicio.tv_sec);
        printf("Tiempo de ejecucion: %f segundos\n", secs);
        
    }

}