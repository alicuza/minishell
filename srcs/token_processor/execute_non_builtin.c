#include "env.h"
#include "minishell.h"

static int	exit_on_issue(char *error_prefix, int error_code)
{
	perror(error_prefix);
	close(0);
	return (error_code);
}

static int	execute_in_child(t_command_ctx *cmd_ctx, char **envp)
{
	char *error_message;
	int error_code;
#ifdef DEBUG
	fprintf(stderr, "\nexecuting in child: %s\n", cmd_ctx->pathname);
#endif
	execve(cmd_ctx->pathname, cmd_ctx->argv, envp);
	if (errno == 2)
		error_code = 127;
	else if (errno == 13)
		error_code = 126;
	else
		error_code = EXIT_FAILURE;
	error_message = ft_strjoin("execve: ", cmd_ctx->pathname);
	if (error_message == NULL)
		return (exit_mem_issue());
	return (exit_on_issue(error_message, error_code));
}

static int	wait_return_status(pid_t pid)
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
		return (exit_on_issue("waitpid", EXIT_FAILURE));
	else
	{
#ifdef DEBUG
		printf("exited, status=%d\n", WEXITSTATUS(wstatus));
#endif
		return (WEXITSTATUS(wstatus));
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


	pid = fork();
	if (pid == -1)
		return (exit_on_issue("fork", EXIT_FAILURE));
	else if (pid == 0)
		return (execute_in_child(cmd_ctx, envp));
	else
		return (wait_return_status(pid));
}
