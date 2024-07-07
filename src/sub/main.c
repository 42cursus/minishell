/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 18:08:47 by abelov            #+#    #+#             */
/*   Updated: 2024/06/28 18:08:48 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	ft_shell_destroy_ctx(t_exec_ctx **ctx)
{
	int			i;
	t_sh_var	*var;

	i = -1;
	while (++i < (*ctx)->env_map.total_elems)
	{
		var = &((t_sh_var *)(*ctx)->env_map.base)[i];
		free((void *)var->k);
		if (var->v != NULL)
			free(var->v);
	}
	free((*ctx)->env_map.base);
	while((*ctx)->argc--)
		free((*ctx)->argv[(*ctx)->argc]);
	free((*ctx)->argv);
	free(*ctx);
	*ctx = NULL;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec_ctx	*global;
	t_obj_arr	*ops;
	t_shell_op	*op;
	char		*tmp;

	if (argc < 2)
		return (EX_NOINPUT);
	signal(SIGSEGV, sigsegv);
	if (do_init_ops(&ops) == -1)
		exit(-1);
	if (do_init(&global, envp) == -1)
		exit(-1);
	global->fdio.out = 1;
	global->argv = ft_split(argv[1], ' ');
	global->argc = ft_get_tab_size(global->argv);
	op = ft_bsearch_obj(&(t_shell_op){.instruction = global->argv[0]}, ops);
	if (op != NULL)
		op->fun(global);
	ft_shell_destroy_ctx(&global);
	free(ops);
	return (EX_OK);
}
