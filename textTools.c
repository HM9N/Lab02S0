#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
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

    if (chars[strlen(chars) - 1] == '\t' || chars[strlen(chars) - 1] == '\a' || chars[strlen(chars) - 1] == '\n' || chars[strlen(chars) - 1] == '\r' || chars[strlen(chars) - 1] == ' ')
    {
        char *charsCopyAux1 = (char *)malloc(sizeof(char) * 100);
        strcpy(charsCopyAux1, chars);
        int j = strlen(chars) - 1;

        for (int i = 0; i < strlen(chars); i++)
        {
            chars[i] = charsCopyAux1[j];
            j--;
        }

        while (chars[0] == '\t' || chars[0] == '\a' || chars[0] == '\n' || chars[0] == '\r' || chars[0] == ' ')
        {
            memmove(&chars[0], &chars[1], strlen(chars));
        }

        char *charsCopyAux2 = (char *)malloc(sizeof(char) * 100);
        strcpy(charsCopyAux2, chars);
        j = strlen(chars) - 1;

        for (int i = 0; i < strlen(chars); i++)
        {
            chars[i] = charsCopyAux2[j];
            j--;
        }

        free(charsCopyAux1);
        free(charsCopyAux2);
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

// Verifica si el texto tiene el simbolo "&"
int isParallelCommand(char *str)
{
    if (strstr(str, "&") == NULL)
    {
        return 0;
    }

    return 1;
}
