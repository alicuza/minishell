/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_simple_command.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:02:33 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/14 01:34:02 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_subshell(t_ctx *c, t_node *cmd_node, t_node *redir_node)
{
	sig_set_default();
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
	{
		cleanup_context(c);
		exit(EXIT_FAILURE);
	}
	redirect_io(c, NULL);
	close_all_fds(c);
	close_heredoc_fds(c);
	c->pid_to_wait = -1;
	execute_list(c, cmd_node->data.command.arg_head_idx);
	cleanup_context(c);
	exit(c->return_status);
}

static int	execute_subshell(t_ctx *c, t_node *cmd_node, t_node *redir_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (msh_error("fork", NULL, strerror(errno)));
	if (pid == 0)
		run_subshell(c, cmd_node, redir_node);
	close_io(c);
	c->pid_to_wait = pid;
	return (EXIT_SUCCESS);
}

static int	handle_empty_command(t_ctx *c, t_command_ctx *command,
		t_node *redir_node)
{
	int	result;

	if (process_redirection(c, redir_node) == EXIT_FAILURE)
		result = EXIT_FAILURE;
	else
	{
		msh_error(NULL, command->argv[0], "command not found");
		result = 127;
	}
	close_io(c);
	return (result);
}

static int	handle_no_command(t_ctx *c, t_command_ctx *command,
		t_node *redir_node)
{
	int	result;

	result = EXIT_SUCCESS;
	if (c->is_pipe && command->argc == 0)
		return (execute_builtin_in_subshell(c, command, no_op, redir_node));
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
		result = EXIT_FAILURE;
	close_io(c);
	return (result);
}

int	execute_simple_command(t_ctx *c, t_node *command_node)
{
	t_node			*arg_node;
	t_node			*redir_node;
	t_command_ctx	command;
	int				result;

	result = EXIT_SUCCESS;
	arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.arg_head_idx);
	redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.redir_head_idx);
	if (command_node->flags & FLAG_SUBSHELL)
		return (execute_subshell(c, command_node, redir_node));
	ft_memset(&command, 0, sizeof(command));
	if (build_command(c, &command, arg_node, redir_node) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (command.argc > 0 && is_empty_str(command.argv[0]))
		result = handle_empty_command(c, &command, redir_node);
	else if (command.pathname == NULL)
		result = handle_no_command(c, &command, redir_node);
	else
		result = command_search_and_execution(c, &command, redir_node);
	free(command.pathname);
	return (result);
}
