#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "textTools.h"

void replaceLineBreak(char **line)
{
    char *p = *line;
    while (*p != '\n')
    {
        p++;
    }
    *p = '\0';
}

int isRedirection(char *str)
{
    if (strstr(str, "<") == NULL)
    {
        return 0;
    }

    return 1;
}
