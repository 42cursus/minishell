/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:55:33 by yublee            #+#    #+#             */
/*   Updated: 2024/06/10 17:44:47 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include "get_next_line.h"

typedef struct s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	void			*item;
}	t_btree;

//create tree
void	expand_tree_pipe(t_btree *root);
void	expand_tree_redirect_l(t_btree *root);
void	expand_tree_redirect_r(t_btree *root);

//create command list
t_list	*get_cmds(t_btree *root);

//tree utils
void	print_node(void *item);
void	free_node(t_btree *root);
t_btree	*create_node(void *item);
void	btree_apply_infix(t_btree *root, void (*applyf)(void *));
void	btree_apply_suffix(t_btree *root, void (*applyf)(t_btree *));

//list utils
void	del(void *item);
void	print_list(void *content);

//utils
void	ft_strtrim_and_free(t_btree *root);

#endif