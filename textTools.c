#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "textTools.h"

// Garantiza que el comando se va a ejecutar sin importar la cantidad de espacios, tabulaciones, etc.
void eliminateCharacters(char *chars)
{
    if (chars[0] == '\t' || chars[0] == '\a' || chars[0] == '\n' || chars[0] == '\r' || chars[0] == ' ')
    {
        while (chars[0] == '\t' || chars[0] == '\a' || chars[0] == '\n' || chars[0] == '\r' || chars[0] == ' ')
        {
            memmove(&chars[0], &chars[1], strlen(chars));
        }
    }
    int i = 0, j = 0, aux = 0;

    while (i < strlen(chars))
    {
        if (chars[i] == '\t' || chars[i] == '\a' || chars[i] == '\n' || chars[i] == '\r' || chars[i] == ' ')
        {
            j = i + 1;
            while (chars[j] == '\t' || chars[j] == '\a' || chars[j] == '\n' || chars[j] == '\r' || chars[j] == ' ')
            {
                memmove(&chars[j], &chars[j + 1], strlen(chars) - j);
            }
        }
        i++;
    }
    
}

void replaceLineBreak(char **line)
{
    char *p = *line;
    while (*p != '\n')
    {
        p++;
    }
    *p = '\0';
}

// Verifica si el texto tiene el simbolo ">"
int isRedirection(char *str)
{
    if (strstr(str, ">") == NULL)
    {
        return 0;
    }

    return 1;
}
