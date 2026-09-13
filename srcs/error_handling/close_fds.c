/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:14:23 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/12 15:14:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_close_fd(int *fd)
{
	if (*fd != -1)
		close(*fd);
	*fd = -1;
}

void	close_io(t_ctx *c)
{
	ft_close_fd(&c->io_fd[0]);
	ft_close_fd(&c->io_fd[1]);
}

void	close_all_fds(t_ctx *c)
{
	close_io(c);
	ft_close_fd(&c->pipe_fd[0]);
	ft_close_fd(&c->pipe_fd[1]);
}
