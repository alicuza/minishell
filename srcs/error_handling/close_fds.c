/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_fds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:14:23 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/13 21:02:43 by nribakov         ###   ########.fr       */
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

void	close_heredoc_fds(t_ctx *c)
{
	t_arena		*commands;
	t_node		*node;
	uint64_t	count;
	uint64_t	i;

	commands = &c->arena[AT_COMMAND];
	count = get_idx_from_offset(commands, commands->offset);
	i = 0;
	while (++i < count)
	{
		node = get_ptr_from_idx(commands, i);
		if (node->type == NODE_REDIR && (node->flags & REDIR_HERE))
			ft_close_fd(&node->data.redir.fd);
	}
}
