/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_return_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:03:02 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/13 21:50:25 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_return_status(int wstatus)
{
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	else if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
}

int	wait_return_status(t_ctx *c)
{
	int		wstatus;
	pid_t	wpid;
	int		result;

	result = EXIT_FAILURE;
	while (1)
	{
		wpid = waitpid(-1, &wstatus, 0);
		if (wpid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno != ECHILD)
			{
				msh_error("waitpid", NULL, strerror(errno));
				return (result);
			}
			break ;
		}
		if (wpid == c->pid_to_wait)
			result = get_return_status(wstatus);
	}
	c->pid_to_wait = -1;
	g_signal = 0;
	return (result);
}
