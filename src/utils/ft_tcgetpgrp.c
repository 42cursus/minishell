/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tcgetpgrp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 00:03:24 by abelov            #+#    #+#             */
/*   Updated: 2025/02/03 00:03:25 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <sys/ioctl.h>

/**
 * The  function  ft_tcgetpgrp() returns the process group ID of the foreground
 * process group on the terminal associated to fd, which must be the controlling
 * terminal of the calling process.
 *
 * @param fd
 * @return
 * 	when fd refers to the controlling terminal of the calling process,
 * 	ft_tcgetpgrp() will return the foreground process group ID of that terminal
 * 	if there is one, and some value larger than 1 that is not presently
 * 	a process group  ID otherwise. When  fd does not refer to the controlling
 * 	terminal of the calling process, -1 is returned, and errno is set.
 */
pid_t	ft_tcgetpgrp(int fd)
{
	pid_t	pgid;

	if (ioctl(fd, TIOCGPGRP, &pgid) == -1)
		pgid = -1;
	return (pgid);
}
