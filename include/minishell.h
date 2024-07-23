/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:55:33 by yublee            #+#    #+#             */
/*   Updated: 2024/07/12 17:43:51 by yublee           ###   ########.fr       */
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
#include "test.h"

# define READ_END 0
# define WRITE_END 1

typedef struct s_btree
{
	struct s_btree	*left;
	struct s_btree	*right;
	void			*item;
}	t_btree;

typedef struct s_info
{
	int			cmd_cnt;
	char		**env;
	int			**fds;
	t_btree		*root;
	t_list		**cmd_list;
	t_ctx		*ctx;
	t_obj_arr	*ops;
}	t_info;

//syntax check
void	syntax_validation(char *str);

//create tree
void	expand_tree_pipe(t_btree *root);
void	expand_tree_redirect(t_btree *root);
char	*ft_leftoverdup(char *str, char *str_sub, char c);
char	*ft_worddup(char *str, char c);

//create command list
t_list	*get_cmds(t_btree *root);

//pipex
void	pipex(t_list **cmd_list, t_btree *root, t_ctx *ctx, t_obj_arr *ops);
void	get_input(t_btree *cmd, int i, t_info info);
void	get_output(t_btree *cmd, int i, t_info info);
int 	child_process(int i, t_list *current, t_info info);
char	**get_argv(char *str, char **env);

//tree utils
void	print_node(void *item);
void	free_node(t_btree *root);
t_btree	*create_node(void *item);
void	btree_apply_infix(t_btree *root, void (*applyf)(void *));
void	btree_apply_suffix(t_btree *root, void (*applyf)(t_btree *));

//list utils
void	del(void *item);
void	print_list(void *content);

//str utils
char	**ft_split_str_with_quotes(char *s, char c);
char	*mask_quoted_part(char *s, char c);
void	ft_strtrim_and_free(t_btree *root);

//utils
void	free_before_exit(t_info	info);
void	exit_with_message(char *str, int exit_no, t_info info);
void	free_str_array(char **array);
int		free_fds(int **fds, int i);

#endif