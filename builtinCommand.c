#include <string.h>
#include "builtinCommand.h"
 
builtinCommand strToCommand(char *strCommand)
{
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(strCommand, commands[i].stringCommand) == 0)
        {
            return commands[i].command;
        }
    }
    return not_command;
}