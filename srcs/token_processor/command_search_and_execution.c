#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

static t_command_function	match_builtin(char *name)
{
	if (ft_strncmp(name, ENV, 4) == EQUAL)
		return (&env);
	else if (ft_strncmp(name, PWD, 4) == EQUAL)
		return (&pwd);
	else if (ft_strncmp(name, EXIT, 5) == EQUAL)
		return (&builtin_exit);
	else if (ft_strncmp(name, CD, 3) == EQUAL)
		return (&cd);
	else if (ft_strncmp(name, EXPORT, 7) == EQUAL)
		return (&builtin_export);
	else if (ft_strncmp(name, UNSET, 6) == EQUAL)
		return (&unset);
	else if (ft_strncmp(name, B_ECHO, 7) == EQUAL)
		return (&echo);
	else
		return (NULL);
}

/*
	TODO nik: 2.9.1.3 Commands with no Command Name
	If a simple command has no command name after word expansion (see 2.9.1.1 Order of Processing), any redirections shall be performed in a subshell environment; If any of the redirections performed in the current shell execution environment fail, the command shall immediately fail with an exit status greater than zero, and the shell shall write an error message indicating the failure.
*/
int	command_search_and_execution(t_ctx *c, t_command_ctx *cmd_ctx)
{
	t_command_function	command;
	int					status;

	command = NULL;
	if (ft_strchr(cmd_ctx->pathname, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->pathname);
		if (command != NULL) //TODO nik: for builtinsdo rederactions and revert them when you done
			return (command(c, cmd_ctx));
		else
		{
			status = get_pathname(c, cmd_ctx);
			if (status == EXIT_FAILURE)
				return (exit_mem_issue());
			else if (status == EXIT_SUCCESS && cmd_ctx->pathname != NULL)
				return (execute_non_builtin(c, cmd_ctx));
			else
				return (127);
		}
	}
	else
		return (execute_non_builtin(c, cmd_ctx));
}
