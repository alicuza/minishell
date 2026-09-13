/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_here_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:53:43 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 21:53:48 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_here_file(t_ctx *c, t_node *redir_node)
{
	ft_close_fd(&c->io_fd[0]);
	c->io_fd[0] = redir_node->data.redir.fd;
	redir_node->data.redir.fd = -1;
	if (c->io_fd[0] == -1)
		return (msh_error("redirection", "here-doc", strerror(EBADF)));
	return (EXIT_SUCCESS);
}
