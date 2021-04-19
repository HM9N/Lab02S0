#ifndef B_COMMAND_H
#define B_COMMAND_H

typedef enum
{
    jason,
    abril,
    diana,
    endup,
    not_command
} builtinCommand;

const static struct
{
    builtinCommand command;
    char *stringCommand;
} commands[] = {
    {jason, "jason"},
    {abril, "abril"},
    {diana, "diana"},
    {endup, "exit"}};

builtinCommand strToCommand(char *strCommand);

#endif