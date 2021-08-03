#ifndef S_PATH_H
#define S_PATH_H

void modifySearchPath(char *searchPath[], char *args[], int *pathCounter);

int searchPaths(char **path, char *args[], int pathCounter, int *pathPosition);

int isPath(char *path);

void executeCommand(char *path, char *args[], int isRed, int countRed, char *redirectionFile, int isParCom, int *forks, int *counforks, int parComCounter);

#endif