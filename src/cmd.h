#ifndef _CMD_H
#define _CMD_H

#include "parser.h"

#define SHELL_EXIT -100

int parse_command(cmd_t *cmd, int level, cmd_t *father);

#endif
