#ifndef _UTILS_H
#define _UTILS_H

#include "parser.h"

#define DIE(assertion, call_description)      \
  do {              \
    if (assertion) {        \
      fprintf(stderr, "(%s, %s, %d): ",  \
        __FILE__, __func__, __LINE__);  \
      perror(call_description);    \
      exit(EXIT_FAILURE);      \
    }            \
  } while (0)

char *ft_get_word(t_wrd *wrd, t_ctx *ctx);
char *get_word(word_t *s, t_ctx *ctx);

char **get_argv(simple_cmd_t *command, int *size, t_ctx *ctx);
char **ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx);
#endif
