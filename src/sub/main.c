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

int	ft_shell_op_cmp(const void *a, const void *b)
{
	const t_shell_op	*op1 = a;
	const t_shell_op	*op2 = b;

	return (ft_strcmp(op1->instruction,  op2->instruction));
}

int	ft_shell_var_cmp(const void *a, const void *b)
{
	const t_sh_var	*var1 = a;
	const t_sh_var	*var2 = b;

	return (ft_strcmp(var1->k,  var2->k));
}

t_sh_var	*ft_shell_env_map_get(const char *key, t_exec_ctx *ctx)
{
	t_sh_var	*found;

	found = ft_bsearch_obj(&(t_sh_var){.k = key}, &ctx->env_map);
	return (found);
}

int	ft_shell_parse_env_map(t_obj_arr *map, char **env_tab)
{
	size_t		i;
	size_t		map_size;
	t_sh_var	*env_map;
	char		*saveptr;
	char		*tmp;

	map_size = ft_get_tab_size((const void **) env_tab);
	env_map = (t_sh_var *)malloc(map_size * (sizeof(t_sh_var)));
	if (!env_map)
		return (-1);
	i = -1;
	while (++i < map_size)
	{
		saveptr = NULL;
		tmp = ft_strdup(env_tab[i]);
		env_map[i].k = ft_strtok_r(tmp, "=", &saveptr);
		env_map[i].attrs = ATT_EXPORTED | ATT_IMPORTED;
		if (saveptr != NULL)
			env_map[i].v = ft_strdup(saveptr);
	}
	map->base = env_map;
	map->elem_size = sizeof(t_sh_var);
	map->total_elems = map_size;
	map->cmp_fun = ft_shell_var_cmp;
	ft_qsort_obj(map);
	return (0);
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
	new->cmp_fun = ft_shell_op_cmp;
	ft_qsort_obj(new);
	*ops = new;
	return (0);
}

int	do_init(t_exec_ctx	**ctx, char **envp)
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
	free((*ctx)->cmdline);
	free((*ctx)->command);
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
	global->cmdline = ft_strdup(argv[1]);
	global->fdio.out = 1;

	tmp = ft_strdup(global->cmdline);
	global->command = ft_strdup(ft_strtok(tmp, " "));
	free(tmp);
	op = ft_bsearch_obj(&(t_shell_op){.instruction = global->command}, ops);
	if (op != NULL)
		op->fun(global);
	ft_shell_destroy_ctx(&global);
	free(ops);
	return (EX_OK);
}
