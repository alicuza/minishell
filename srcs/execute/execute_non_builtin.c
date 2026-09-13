/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_non_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:59:22 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/14 01:33:04 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx,
		t_node *redir_node)
{
	t_error	e;
	char	**envp;

	sig_set_default();
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
	{
		cleanup_shell(c, cmd_ctx, NULL);
		exit(EXIT_FAILURE);
	}
	redirect_io(c, cmd_ctx);
	close_all_fds(c);
	close_heredoc_fds(c);
	envp = env_to_envp(&c->env);
	if (envp == NULL)
		handle_mem_error(c, cmd_ctx, NULL);
	errno = 0;
	execve(cmd_ctx->pathname, cmd_ctx->argv, envp);
	if (errno == ENOENT)
		e = (t_error){cmd_ctx->argv[0], "command not found", 127};
	else
		e = (t_error){cmd_ctx->pathname, strerror(errno), 126};
	msh_exit(c, cmd_ctx, &e, envp);
	return (EXIT_FAILURE);
}

int	execute_non_builtin(t_ctx *c, t_command_ctx *cmd_ctx, t_node *redir_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (msh_error("fork", NULL, strerror(errno)));
	if (pid == 0)
		return (execute_in_child(c, cmd_ctx, redir_node));
	c->pid_to_wait = pid;
	close_io(c);
	return (EXIT_SUCCESS);
}
