#include "minishell.h"

static void	run_subshell(t_ctx *c, t_node *cmd_node, t_node *redir_node)
{
	c->is_pipe = false;
	c->pid_to_wait = -1;
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
	{
		cleanup(c);
		exit(EXIT_FAILURE);
	}
	close_io(c);
	execute_list(c, cmd_node->data.command.arg_head_idx);
	cleanup(c);
	exit(c->return_status);
}

static int	execute_subshell(t_ctx *c, t_node *cmd_node, t_node *redir_node)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), EXIT_FAILURE);
	if (pid == 0)
		run_subshell(c, cmd_node, redir_node);
	close_io(c);
	c->pid_to_wait = pid;
	return (EXIT_SUCCESS);
}

int	execute_simple_command(t_ctx *c, t_node *command_node)
{
	t_node			*arg_node;
	t_node			*redir_node;
	t_command_ctx	command;
	int result;

	result = EXIT_SUCCESS;
	arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.arg_head_idx);
	redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			command_node->data.command.redir_head_idx);
	if (command_node->flags & FLAG_SUBSHELL)
		return (execute_subshell(c, command_node, redir_node));
	if (build_command_arena(c, &command, arg_node, redir_node) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (command.pathname == NULL)
	{
		if (process_redirection(c, redir_node) == EXIT_FAILURE)
			result = EXIT_FAILURE;
	}
	else
		result = command_search_and_execution(c, &command,
				redir_node);
	free(command.pathname);
	return (result);
}
