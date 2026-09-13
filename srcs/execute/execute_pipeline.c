/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 21:03:20 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/14 01:37:23 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_pipe_error(t_ctx *c)
{
	msh_error("pipe", NULL, strerror(errno));
	cleanup_context(c);
	if (c->pid_to_wait != -1)
		(void)wait_return_status(c);
	exit(EXIT_FAILURE);
}

static int	pipe_and_execute_simple_command(t_ctx *c, t_node *command_node)
{
	int	result;

	if (pipe(c->pipe_fd) == -1)
	{
		handle_pipe_error(c);
		return (EXIT_FAILURE);
	}
	c->io_fd[1] = c->pipe_fd[1];
	c->pipe_fd[1] = -1;
	result = execute_simple_command(c, command_node);
	close_io(c);
	c->io_fd[0] = c->pipe_fd[0];
	c->pipe_fd[0] = -1;
	return (result);
}

static int	wait_child(t_ctx *c, int result)
{
	if (result == EXIT_SUCCESS)
		return (wait_return_status(c));
	(void)wait_return_status(c);
	return (result);
}

int	execute_pipeline(t_ctx *c, t_node *pipeline_node)
{
	t_node	*command_node;
	int		result;

	result = EXIT_SUCCESS;
	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			pipeline_node->data.pipeline.command_head_idx);
	while (command_node->type == NODE_COMMAND)
	{
		if (command_node->next_idx != 0)
		{
			c->is_pipe = true;
			result = pipe_and_execute_simple_command(c, command_node);
		}
		else
			result = execute_simple_command(c, command_node);
		if (c->should_exit)
			return (wait_return_status(c), result);
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				command_node->next_idx);
	}
	c->is_pipe = false;
	if (c->pid_to_wait != -1)
		result = wait_child(c, result);
	return (result);
}
