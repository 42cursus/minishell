#ifndef _CMD_H
#define _CMD_H

#include "parser.h"
#include "minishell.h"

#define SHELL_EXIT -100

int traverse_and_exec_the_ast(cmd_t *c, int level, cmd_t *father);

#endif
