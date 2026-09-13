/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_in_subshell.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:37:12 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/14 00:41:01 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command, t_node *redir_node)
{
	int		result_code;

	result_code = 0;
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
	{
		cleanup_shell(c, cmd_ctx, NULL);
		exit(EXIT_FAILURE);
	}
	redirect_io(c, cmd_ctx);
	close_all_fds(c);
	close_heredoc_fds(c);
	result_code = command(c, cmd_ctx);
	cleanup_shell(c, cmd_ctx, NULL);
	exit(result_code);
	return (result_code);
}

int	execute_builtin_in_subshell(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command, t_node *redir_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (msh_error("fork", NULL, strerror(errno)));
	if (pid == 0)
		return (execute_in_child(c, cmd_ctx, command, redir_node));
	c->pid_to_wait = pid;
	close_io(c);
	return (EXIT_SUCCESS);
}
