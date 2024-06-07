/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yublee <yublee@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:55:33 by yublee            #+#    #+#             */
/*   Updated: 2024/05/05 01:59:42 by yublee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"
# include "get_next_line.h"
# define READ_END 0
# define WRITE_END 1

typedef struct s_info
{
	char	*input;
	char	*output;
	int		cmd_cnt;
	char	**env;
	int		here_doc;
	char	*h_end;
	int		**fds;
}	t_info;

int		pipex(t_info info, char **argv);
char	**get_args(char *argv, char **env, t_info info);
void	exit_with_error(char *str, int exit_no, t_info info);
void	free_str_array(char **array);
int		free_fds(int **fds, int i);

#endif