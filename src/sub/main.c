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

t_sh_var	*ft_shell_parse_env_map(char **env_tab)
{
	size_t	map_size;
	t_sh_var	*env_map;
	char	*saveptr;

	map_size = ft_get_tab_size((const void **) env_tab);
	env_map = (t_sh_var *)malloc(map_size);
	if (!env_map)
		return (NULL);
	while (map_size--)
	{
		saveptr = NULL;
		env_map[map_size].k = ft_strtok_r(ft_strdup(env_tab[map_size]),
										  "=", &saveptr);
		env_map[map_size].attrs = ATT_EXPORTED | ATT_IMPORTED;
		if (saveptr != NULL)
			env_map[map_size].v = ft_strdup(saveptr);
	}
	return (env_map);
}

int	ft_shell_op_cmp(const void *a, const void *b)
{
	const t_shell_op	*op1 = a;
	const t_shell_op	*op2 = b;

	return (ft_strcmp(op1->instruction,  op2->instruction));
}

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
	ft_qsort(new->base, new->total_elems,
			 new->elem_size, ft_shell_op_cmp);
	*ops = new;
	return (0);
}

int	do_init(t_exec_ctx	**ctx, char **envp)
{
	t_exec_ctx	*new;

	new = (t_exec_ctx *) malloc(sizeof(t_exec_ctx));
	if (!new)
		return (-1);
	new->env_map = ft_shell_parse_env_map(envp);
	*ctx = new;
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_exec_ctx	*global;
	t_obj_arr	*ops;
	t_shell_op	*op;
	int			errcode;
	char		*tmp;

	if (argc < 2)
		return (EX_NOINPUT);
	signal(SIGSEGV, sigsegv);
	errcode = do_init_ops(&ops);
	if (errcode == -1)
		exit(errcode);
	errcode = do_init(&global, envp);
	if (errcode == -1)
		exit(errcode);
	global->cmdline = argv[1];
	global->fdio.out = 1;

	tmp = ft_strdup(global->cmdline);
	global->command = ft_strdup(ft_strtok(tmp, " "));
	free(tmp);
	op = ft_bsearch(&(t_shell_op){.instruction = global->command},
									ops, ft_shell_op_cmp);
	if (op != NULL)
		op->fun(global);
	return (EX_OK);
}
