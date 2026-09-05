#include "env.h"
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

static int	handle_dup2_error(t_ctx *c, t_command_ctx *cmd_ctx, char **envp)
{
	ft_putstr_fd("dup2: ", STDERR_FILENO);
	perror("");
	child_cleanup_all(c, cmd_ctx, envp);
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

// TODO: here we need to reset sigaction for SIGINT and SIGQUITE since we need to interapt child
static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx, char **envp)
{
#ifdef DEBUG
	fprintf(stderr, "\nexecuting in child: %s\n", cmd_ctx->pathname);
#endif
	// TODO nik: process_redirection();
	sig_set_default();
	ft_close_fd(&c->pipe_fd[0]);
	if(c->io_fd[0] != -1)
	{
		if(dup2(c->io_fd[0], 0) < 0)
			return (handle_dup2_error(c, cmd_ctx, envp));
		ft_close_fd(&c->io_fd[0]);
	}
	if(c->io_fd[1] != -1)
	{
		if(dup2(c->io_fd[1], 1) < 0)
			return (handle_dup2_error(c, cmd_ctx, envp));
		ft_close_fd(&c->io_fd[1]);
	}
	execve(cmd_ctx->pathname, cmd_ctx->argv, envp);
	return(exit_child(c, cmd_ctx, envp));
}

//TODO nik: don't wait for each command to finish
static int	wait_return_status(t_ctx *c, pid_t pid)
{
	int wstatus;
	pid_t wpid;

#ifdef DEBUG
	fprintf(stderr, "\nchild pid=%jd\n", (intmax_t)pid);
#endif
	wpid = waitpid(pid, &wstatus, 0);
	while (wpid == -1 && errno == EINTR)
		wpid = waitpid(pid, &wstatus, 0);
	if (wpid == -1)
		return (exit_on_issue("waitpid"));
	else
	{
#ifdef DEBUG
		if (WIFEXITED(wstatus))
			printf("exited, status=%d\n", WEXITSTATUS(wstatus));
		else if (WIFSIGNALED(wstatus))
			printf("signaled, sig=%d\n", WTERMSIG(wstatus));
#endif
		if (WIFSIGNALED(wstatus))
			c->return_status = 128 + WTERMSIG(wstatus);
		else if (WIFEXITED(wstatus))
			c->return_status = WEXITSTATUS(wstatus);
		g_signal = 0;
		return (EXIT_SUCCESS);
	}
}

int	execute_non_builtin(t_ctx *c, t_command_ctx *cmd_ctx)
{
	char **envp;
	pid_t pid;

	envp = env_to_envp(&c->env);
	if (envp == NULL)
		return (exit_mem_issue());
#ifdef DEBUG
	fprintf(stderr, "\nexecute_non_builtin: %s\n", cmd_ctx->pathname);
#endif
	pid = fork(); // TODO nik: add to context the pids
	if (pid == -1)
	{
		perror("fork");
		close(0);
		free_str_arr(envp);
		return (EXIT_FAILURE); 
	}
	if (pid == 0)
		return (execute_in_child(c, cmd_ctx, envp));
	close_io(c);
	free_str_arr(envp);
	return (wait_return_status(c, pid));
}
