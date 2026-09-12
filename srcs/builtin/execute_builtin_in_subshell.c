#include "minishell.h"

static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command, t_node *redir_node)
{
	int		result_code;
	t_error	e;

	result_code = 0;
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
	{
		cleanup_shell(c, cmd_ctx, NULL);
		exit(EXIT_FAILURE);
	}
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
	close_all_fds(c);
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
