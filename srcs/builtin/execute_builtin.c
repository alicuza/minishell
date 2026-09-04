#include "minishell.h"

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

int	execute_in_subshell(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(0);
		free_str_arr(envp);
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		return (execute_in_child(c, cmd_ctx, command));
	close_io(c);
	return (wait_return_status(c, pid));
}

static int	execute(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	int	result_code;
	int	orig_fd[2];

	result_code = 0;
	orig_fd[0] = dup(0);
	orig_fd[1] = dup(1);
	if (c->io_fd[0] != -1)
	{
		if(dup2(c->io_fd[0], 0) < 0)
			return(); //TODO nik: return 
		ft_close_fd(&c->io_fd[0]);
	}
	if (c->io_fd[1] != -1)
	{
		if(dup2(c->io_fd[1], 1) < 0)
			return();
		ft_close_fd(&c->io_fd[1]);
	}
	result_code = command(c, cmd_ctx);
	dup2(orig_fd[0], 0);
	dup2(orig_fd[1], 1);
	close(orig_fd[0]);
	close(orig_fd[1]);
	return (result_code);
}

int	execute_builtin(t_ctx *c, t_command_ctx *cmd_ctx,
		t_command_function command)
{
	if (c->is_pipe)
		execute_in_subshell(c, cmd_ctx, command);
	else
		execute(c, cmd_ctx, command);
	return (0);
}
