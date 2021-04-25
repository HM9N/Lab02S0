#include <stdlib.h>
#include <string.h>
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