#include "minishell.h"

void	execute_pipeline(t_ctx *c, t_node *pipeline_node)
{
	t_node		*command_node;
	int			result;

	c->is_pipe = true;
	result = EXIT_SUCCESS;
	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			pipeline_node->data.pipeline.command_head_idx);
	while (command_node->type == NODE_COMMAND)
	{
		if (command_node->next_idx != 0)
		{
			if (pipe(c->pipe_fd) == -1)
			{
				msh_error("pipe", NULL, strerror(errno));
				close_io(c);
				c->return_status = 1;
				c->is_pipe = false;
				if (c->pid_to_wait != -1)
					wait_return_status(c);
				return ;
			}
			c->io_fd[1] = c->pipe_fd[1];
			c->pipe_fd[1] = -1;
			result = execute_simple_command(c, command_node);
			if (c->should_exit)
			{
				close_all_fds(c);
				c->is_pipe = false;
				return ;
			}
			close_io(c);
			c->io_fd[0] = c->pipe_fd[0];
			c->pipe_fd[0] = -1;
		}
		else
		{
			result = execute_simple_command(c, command_node);
			if (c->should_exit)
			{
				close_all_fds(c);
				c->is_pipe = false;
				return ;
			}
		}
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				command_node->next_idx);
	}
	c->return_status = result;
	if (c->pid_to_wait != -1)
		wait_return_status(c);
	c->is_pipe = false;
}
