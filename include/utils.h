/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 00:23:19 by abelov            #+#    #+#             */
/*   Updated: 2025/01/21 00:23:19 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "parser.h"

# define DIE(assertion, call_description)      \
  do {              \
    if (assertion) {        \
      fprintf(stderr, "(%s, %s, %d): ",  \
        __FILE__, __func__, __LINE__);  \
      perror(call_description);    \
      exit(EXIT_FAILURE);      \
    }            \
  } while (0)

char	*ft_get_word(t_wrd *wrd, t_ctx *ctx);
char	*get_word(word_t *s, t_ctx *ctx);
char	**get_argv(scmd_t *command, int *size, t_ctx *ctx);
char	**ft_get_argv(t_cmd_node *cmd, int *size, t_ctx *ctx);

#endif //UTILS_H
