/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:41:03 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/14 00:41:12 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirect_io(t_ctx *c, t_command_ctx *cmd_ctx)
{
	t_error	e;

	if (c->io_fd[0] != -1)
	{
		if (dup2(c->io_fd[0], 0) < 0)
		{
			e = (t_error){"dup2", strerror(errno), 1};
			msh_exit(c, cmd_ctx, &e, NULL);
		}
	}
	if (c->io_fd[1] != -1)
	{
		if (dup2(c->io_fd[1], 1) < 0)
		{
			e = (t_error){"dup2", strerror(errno), 1};
			msh_exit(c, cmd_ctx, &e, NULL);
		}
	}
}
