/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_perrorf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:59:32 by abelov            #+#    #+#             */
/*   Updated: 2025/02/08 23:59:32 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"

void ft_perrorf(const char *fmt, ...)
{
	char	buf[256];
	va_list	args;

	va_start(args, fmt);
	ft_vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	perror(buf);
}