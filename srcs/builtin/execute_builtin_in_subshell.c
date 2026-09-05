#include "minishell.h"

static void	close_io(t_ctx *c)
{
	ft_close_fd(&c->io_fd[0]);
	ft_close_fd(&c->io_fd[1]);
}

static int	exit_on_issue(char *error_prefix) // TODO nik make sure it clean ups correctly
{
	perror(error_prefix);
	close(0);
	return (EXIT_FAILURE);
}

void	child_cleanup(t_ctx *c, t_command_ctx *cmd_ctx)
{
	cleanup(c);
	free(cmd_ctx->pathname);
	free(cmd_ctx->argv);
}

static int	handle_dup2_error(t_ctx *c, t_command_ctx *cmd_ctx, int error_code)
{
	ft_putstr_fd("dup2: ", STDERR_FILENO);
	perror("");
	child_cleanup(c, cmd_ctx);
	exit(EXIT_FAILURE);
	return (error_code);
}

// TODO: here we need to reset sigaction for SIGINT and SIGQUITE since we need to interapt child
static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	int	result_code;

	result_code = 0;
	ft_close_fd(&c->pipe_fd[0]);
	if (c->io_fd[0] != -1)
	{
		if (dup2(c->io_fd[0], 0) < 0)
			return (handle_dup2_error(c, cmd_ctx, EXIT_FAILURE));
		ft_close_fd(&c->io_fd[0]);
	}
	if (c->io_fd[1] != -1)
	{
		if (dup2(c->io_fd[1], 1) < 0)
			return (handle_dup2_error(c, cmd_ctx, EXIT_FAILURE));
		ft_close_fd(&c->io_fd[1]);
	}
	result_code = command(c, cmd_ctx);
	child_cleanup(c, cmd_ctx);
	exit(result_code);
	return (result_code);
}

static int	wait_return_status(t_ctx *c, pid_t pid)
{
	int		wstatus;
	pid_t	wpid;

#ifdef DEBUG
	fprintf(stderr, "\nchild pid=%jd\n", (intmax_t)pid);
#endif
	wpid = waitpid(pid, &wstatus, 0);
	while (wpid != -1 && !WIFEXITED(wstatus))
	{
		wpid = waitpid(pid, &wstatus, 0);
	}
	if (wpid == -1)
		return (exit_on_issue("waitpid"));
	else
	{
#ifdef DEBUG
		printf("exited, status=%d\n", WEXITSTATUS(wstatus));
#endif
		c->return_status = WEXITSTATUS(wstatus);
		return (EXIT_SUCCESS);
	}
}

int	execute_builtin_in_subshell(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(0);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		return (execute_in_child(c, cmd_ctx, command));
	close_io(c);
	return (wait_return_status(c, pid));
}
