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

/**
 * ft_tcsetpgrp() makes the process group with process group ID pgrp
 * 	the foreground process group on the terminal associated to fd,
 * 	which must be the controlling terminal of the calling process,
 * 	and still be associated with its session.
 *
 * If ft_tcsetpgrp() is called by a member of a background process group
 * 	in its session, and the calling process is not blocking or ignoring SIGTTOU,
 * 	a SIGTTOU signal is sent to all members of this background process group.
 *
 * 	SO, DONT DO IT AT HOME!
 *
 * @param fd
 * @param pgrp_id
 * @return
 */
int	ft_tcsetpgrp(int fd, pid_t pgrp_id)
{
	return (ioctl(fd, TIOCSPGRP, &pgrp_id));
}
