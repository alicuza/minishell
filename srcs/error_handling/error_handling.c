#include "minishell.h"

/*
In builtin export only mem issue can happen.
In cs:
 - If to many arg, log message, return Failure and continue the next one
 -If nessesary env var is missing, logs error return failure contine execution

In case mem issue,
	currently writs message and closes input for gracefull clean up at the end of main and return exit_failure for builtin
- todo maybe stop next comand in pipeline processing
-

*/

// TODO nik: how we exit and make sure everething is closed (fd)
int	exit_mem_issue(void)
{
	perror("Memory allocation error");
	close(0);
	return (EXIT_FAILURE);
}

int	handle_redirection_error(t_ctx *c, char *filename, int error_code)
{
	(void)c;
	ft_putstr_fd("redirection: ", STDERR_FILENO);
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	perror("");
	return (error_code);
}

int	handle_builtin_error(t_ctx *c, char *error_prefix, int error_code)
{
	perror(error_prefix);
	if (c->is_interactive == false)
		close(0);
	return (error_code);
}

void	child_cleanup_all(t_ctx *c, t_command_ctx *cmd_ctx, char **envp)
{
	cleanup(c);
	free(cmd_ctx->pathname);
	free(cmd_ctx->argv);
	free_str_arr(envp);
}

int	exit_child(t_ctx *c, t_command_ctx *cmd_ctx, char **envp)
{
	char	*error;
	int		error_code;

	error_code = errno;
	error = ft_strjoin("execve: ", cmd_ctx->pathname);
	child_cleanup_all(c, cmd_ctx, envp);
	if (error == NULL)
		perror("Memory allocation error");
	else
	{
		errno = error_code;
		perror(error);
	}
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}
