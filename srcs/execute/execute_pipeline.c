#include "minishell.h"

int	pipe_and_execute_simple_command(t_ctx *c, t_node *command_node)
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
	ft_close_fd(&c->io_fd[0]);
	ft_close_fd(&c->io_fd[1]);
	c->io_fd[0] = c->pipe_fd[0];
	c->pipe_fd[0] = -1;
	return (result);
}

int	execute_pipeline(t_ctx *c, t_node *pipeline_node)
{
	t_node	*command_node;
	int		result;

	result == EXIT_SUCCESS;
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
			return (result);
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				command_node->next_idx);
	}
	c->is_pipe = false;
	if (result == EXIT_SUCCESS && c->pid_to_wait != -1)
		return (wait_return_status(c));
	else
		return (result);
}
