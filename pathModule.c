#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "pathModule.h"

int isPath(char *path)
{
    if (!access("/bin/ls", X_OK))
    {
        return 1;
    }
    return 0;
}

int searchAndExecute(char **path, char *args[], int pathCounter)
{
    /*     printf("%s ", path[0]);
    printf("%s ", args[0]);
    printf("%d ", pathCounter); */
    int exists = 0;

    int i = 0;

    while (i < pathCounter)
    {
        exists = isPath(path[i]);
        i++;
    }

    if (exists == 1)
    {
        return 1;
    }

    return 0;
}

void executeCommand()
{
    printf("Soy el proceso padre %d \n", (int)getpid());
    int rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        printf("Soy el proceso hijo %d \n", (int)getpid());
    }
    else
    {
        int rc_wait = wait(NULL);
        printf("Hola, al fin me dejan ejecutar (soy el proceso padre) %d ", (int)getpid());
    }
}