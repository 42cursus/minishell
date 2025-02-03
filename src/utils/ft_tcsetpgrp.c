/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tcsetpgrp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:33:07 by abelov            #+#    #+#             */
/*   Updated: 2025/02/01 16:33:07 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <unistd.h>

/* Set the foreground process group ID of FD set PGRP_ID.  */
int	ft_tcsetpgrp(int fd, pid_t pgrp_id)
{
	return ioctl(fd, TIOCSPGRP, &pgrp_id);
}

