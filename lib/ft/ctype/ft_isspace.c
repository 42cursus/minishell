/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 00:09:12 by abelov            #+#    #+#             */
/*   Updated: 2024/07/17 00:09:12 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c)
{
	return ((c == ' ') || (c == '\t')
			|| (c == '\n') || (c == '\v') || (c == '\f') || (c == '\r'));
}