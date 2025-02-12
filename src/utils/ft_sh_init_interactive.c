/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sh_init_interactive.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 16:49:50 by abelov            #+#    #+#             */
/*   Updated: 2025/02/12 16:49:51 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * https://stackoverflow.com/questions/61452938/
 * https://docs.rtems.org/
 * 	releases/4.5.1-pre3/toolsdoc/gdb-5.0-docs/readline/readline00030.html
 */
int	ft_sh_init_interactive(t_ctx **ctx, char **envp)
{
	t_ctx	*new;

	new = (t_ctx *)ft_calloc(1, sizeof(t_ctx));
	if (!new)
	{
		errno = ENOMEM;
		return (-1);
	}
	new->ps0 = RL_RESET"";
	new->ps1 = RL_RESET"[%d]"RL_GREEN"%s"RL_RESET"@"RL_BLUE"%s"RL_RESET"-> ";
	new->ps2 = RL_RESET"> ";
	new->cmd_name = getenv("_");
	new->last_status_code = 0;
	ft_sh_init_builtin_ops(&new->ops);
	ft_sh_parse_env_map(&new->env_map, envp);
	*ctx = global_ctx(new, SET_VAL);
	if (ft_sh_init_term(*ctx))
		exit(((void)ft_sh_destroy_ctx(*ctx), -1));
	ft_set_signal_handlers(*ctx);
	return (0);
}
