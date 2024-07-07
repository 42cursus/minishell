/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 23:04:03 by abelov            #+#    #+#             */
/*   Updated: 2024/07/04 23:04:04 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int do_init_ops(t_obj_arr **ops)
{
	t_obj_arr			*new;
	static t_shell_op	builtins[] = {
			{.instruction = "pwd", .fun = ft_pwd},
			{.instruction = "env", .fun = ft_env},
			{.instruction = "cd", .fun = ft_chdir},
			{.instruction = "echo", .fun = ft_echo},
			{.instruction = "export", .fun = ft_export},
			{.instruction = "unset", .fun = ft_unset}
	};

	new = (t_obj_arr *) malloc(sizeof(t_obj_arr));
	if (!new)
		return (-1);
	new->base = builtins;
	new->elem_size = sizeof(builtins[0]);
	new->total_elems = sizeof(builtins) / sizeof(builtins[0]);
	new->cmp_fun = ft_shell_op_cmp;
	ft_qsort_obj(new);
	*ops = new;
	return (0);
}

int do_init(t_exec_ctx **ctx, char **envp)
{
	t_exec_ctx	*new;

	new = (t_exec_ctx *) malloc(sizeof(t_exec_ctx));
	if (!new)
		return (-1);
	ft_shell_parse_env_map(&new->env_map, envp);
	new->env_map.cmp_fun = ft_shell_var_cmp;
	ft_qsort_obj(&new->env_map);
	*ctx = new;
	return (0);
}
