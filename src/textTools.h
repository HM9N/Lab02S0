#ifndef TEXT_TOOLS_H
#define TEXT_TOOLS_H

void eliminateCharacters(char *chars);

void replaceLineBreak(char **line);

int isRedirection(char *str);

int isParallelCommand(char *str);

int formatParallelCommand(char *str, char *strAuxParCom, char **commands);

#endif