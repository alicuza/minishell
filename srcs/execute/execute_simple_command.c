#include "minishell.h"

void	execute_simple_command(t_ctx *c, t_node *command_node)
{
	t_node			*arg_node;
	t_node			*redir_node;
	t_command_ctx	command;

	arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.arg_head_idx);
	redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.redir_head_idx);
	if (build_command(c, &command, arg_node) == EXIT_FAILURE)
	{
		c->return_status = 1;
		return ;
	}
	if (command.pathname == NULL)
	{
		if (process_redirection(c, redir_node) == EXIT_FAILURE)
			c->return_status = 1;
	}
	else
		c->return_status = command_search_and_execution(c, &command,
				redir_node);
	free(command.pathname);
	free(command.argv);
}
