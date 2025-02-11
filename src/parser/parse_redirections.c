/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbelcher <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:48:08 by sbelcher          #+#    #+#             */
/*   Updated: 2025/02/10 17:48:11 by sbelcher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	list_append(t_wrd **list, t_wrd *redir)
{
	t_wrd	*curr;

	if (list == NULL)
		return ;
	if (*list == NULL)
		*list = redir;
	else
	{
		curr = *list;
		while (curr->next_word != NULL)
			curr = curr->next_word;
		curr->next_word = redir;
	}
}

void	find_redir_list(t_wrd *redir, t_token_type type, t_cmd_node *cmd)
{
	if (type == T_REDIRECT_STDOUT || type == TOKEN_APPEND)
		list_append(&cmd->redirects_out, redir);
	else if (type == TOKEN_REDIRECT_STDIN || type == TOKEN_HERE_DOC)
		list_append(&cmd->redirects_in, redir);
	else if (type == TOKEN_REDIRECT_STDERR || type == TOKEN_APPEND_2)
		list_append(&cmd->redirects_err, redir);
	else if (type == TOKEN_REDIRECT_IN_2 || type == TOKEN_HERE_DOC_2)
		list_append(&cmd->redirects_err_in, redir);
}

void	parse_redirection(int *tp, t_ast_node *p, t_ctx *ctx, t_lexer *l)
{
	t_wrd			*redir;
	t_token_type	rt;
	bool			hereexpand;
	t_hd_entry		*entry;

	rt = l->tok.t[*tp]->type;
	hereexpand = l->tok.t[*tp]->hereexpand;
	skip_blanks(l->tok.t, tp, NULL, l);
	if (l->tok.t[*tp]
		&& (l->tok.t[*tp]->type == T_WORD || l->tok.t[*tp]->type == T_VAR))
	{
		redir = ft_calloc(sizeof(t_wrd), 1);
		create_wrd(redir, l->tok.t[*tp], rt);
		skip_blanks(l->tok.t, tp, redir, l);
		if (rt == TOKEN_HERE_DOC || rt == TOKEN_HERE_DOC_2)
		{
			l->err = here_doc_cat(redir, l);
			entry = &ctx->hd.entries[ctx->hd.ss++];
			if (l->err == 0)
				l->err = create_here_file(redir, entry, hereexpand);
		}
		find_redir_list(redir, rt, p->cmd);
	}
	else
		l->err = NO_REDIR_TARGET;
}
