#ifndef B_COMMAND_H
#define B_COMMAND_H

typedef enum
{
    cd,
    path,
    endup,
    not_command
} builtinCommand;

const static struct
{
    builtinCommand command;
    char *stringCommand;
} commands[] = {
    {cd, "cd"},
    {path, "path"},
    {endup, "exit"}};

builtinCommand strToCommand(char *strCommand);

#endif