#include "env.h"
#include "minishell.h"

void	close_io(t_ctx *c)
{
	ft_close_fd(&c->io_fd[0]);
	ft_close_fd(&c->io_fd[1]);
}

static int	exit_on_issue(char *error_prefix)
{
	perror(error_prefix);
	close(0);
	return (EXIT_FAILURE);
}

static int	execute_in_child(t_ctx *c, t_command_ctx *cmd_ctx, char **envp)
{
#ifdef DEBUG
	fprintf(stderr, "\nexecuting in child: %s\n", cmd_ctx->pathname);
#endif
	// TODO nik: process_redirection();
	ft_close_fd(&c->pipe_fd[0]);
	if(c->io_fd[0] != -1)
	{
		dup2(c->io_fd[0], 0); // TODO nik: what on error
		ft_close_fd(&c->io_fd[0]);
	}
	if(c->io_fd[1] != -1)
	{
		dup2(c->io_fd[1], 1);
		ft_close_fd(&c->io_fd[1]);
	}
	execve(cmd_ctx->pathname, cmd_ctx->argv, envp);
	return(exit_child(c, cmd_ctx, envp));
}
//TODO nik: The return status (see Exit Status) of a simple command is its exit status as provided by the POSIX 1003.1 waitpid function, or 128+n if the command was terminated by signal n.
static int	wait_return_status(t_ctx *c, pid_t pid)
{
	int wstatus;
	pid_t wpid;

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

int	execute_non_builtin(t_ctx *c, t_command_ctx *cmd_ctx)
{
	char **envp;
	pid_t pid;

	if (env_update_with_copy(&c->env, _, cmd_ctx->pathname) == EXIT_FAILURE)
		return (exit_mem_issue());
	envp = env_to_envp(&c->env);
	if (envp == NULL)
		return (exit_mem_issue());
#ifdef DEBUG
	fprintf(stderr, "\nexecute_non_builtin: %s\n", cmd_ctx->pathname);
#endif
	pid = fork(); // TODO nik: exit / or ignore if ERESTARTNOINTR + add to context the pids
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
