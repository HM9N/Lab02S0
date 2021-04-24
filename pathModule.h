#ifndef S_PATH_H
#define S_PATH_H

int searchPaths(char **path, char *args[], int pathCounter, int *pathPosition);

int isPath(char *path);

void executeCommand(char *path, char *args[]);

#endif