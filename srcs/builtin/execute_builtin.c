#include "minishell.h"

static int	handle_dup_error(t_ctx *c, t_command_ctx *cmd_ctx, int saved[2])
{
	msh_error("dup", NULL, strerror(errno));
	cleanup_shell(c, cmd_ctx, NULL);
	ft_close_fd(&saved_fds[0]);
	ft_close_fd(&saved_fds[1]);
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

static int	save_fds(t_ctx *c, t_command_ctx *cmd_ctx, int saved[2])
{
	saved[0] = -1;
	saved[1] = -1;
	saved[0] = dup(0);
	if (saved[0] < 0)
		return (handle_dup_error(c, cmd_ctx, saved));
	saved[1] = dup(1);
	if (saved[1] < 0)
		return (handle_dup_error(c, cmd_ctx, saved));
	return (EXIT_SUCCESS);
}

int	reset_to_saved_fd(t_ctx *c, t_command_ctx *cmd_ctx, int saved[2])
{
	struct stat	buf;

	if (fstat(STDIN_FILENO, &buf) != -1)
		if (dup2(saved[0], 0) < 0)
			return (handle_dup_error(c, cmd_ctx, saved));
	if (fstat(STDOUT_FILENO, &buf) != -1)
		if (dup2(saved[1], 1) < 0)
			return (handle_dup_error(c, cmd_ctx, saved));
	ft_close_fd(&saved[0]);
	ft_close_fd(&saved[1]);
	return (EXIT_SUCCESS);
}

static int	execute(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	int	result_code;
	int	saved[2];

	result_code = 0;
	if (save_fds(c, cmd_ctx, saved) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (c->io_fd[0] != -1)
	{
		if (dup2(c->io_fd[0], 0) < 0)
			return (handle_dup_error(c, cmd_ctx, saved));
		ft_close_fd(&c->io_fd[0]);
	}
	if (c->io_fd[1] != -1)
	{
		if (dup2(c->io_fd[1], 1) < 0)
			return (handle_dup_error(c, cmd_ctx, saved));
		ft_close_fd(&c->io_fd[1]);
	}
	result_code = command(c, cmd_ctx);
	if (reset_to_saved_fd(c, cmd_ctx, saved) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (result_code);
}

int	execute_builtin(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command, t_node *redir_node)
{
	int	result;

	if (c->is_pipe)
		return (execute_builtin_in_subshell(c, cmd_ctx, command, redir_node));
	else
	{
		if (process_redirection(c, redir_node) == EXIT_FAILURE)
				return (1);
		result = execute(c, cmd_ctx, command);
		close_io(c);
		return (result);
	}
}
