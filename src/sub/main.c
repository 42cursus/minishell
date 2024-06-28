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

void	do_init(t_ops *t)
{
	static t_fun const functions[] = {
			ft_pwd
	};

	signal(SIGSEGV, sigsegv);
	t->size = sizeof(functions) / sizeof(functions[0]);
	t->functions = functions;
}

int	main(void)
{
	int i;
	t_ops	t;

	do_init(&t);
	i = 0;
	while (i < (int) t.size)
		t.functions[i++]();
	return (EX_OK);
}
