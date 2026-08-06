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
	Directly follows 2.9.1.4 Command Search and Execution
	TODO nik: maybe remember its location and need not search for the utility again unless the PATH variable has been the subject of an assignment. If the remembered location fails for a subsequent invocation,
		the shell shall repeat the search to find the new location for the utility,
		if any.
*/
int	command_search_and_execution(t_ctx *c, t_command_ctx *cmd_ctx)
{
	t_command_function	command;
	int					status;

	command = NULL;
	if (ft_strchr(cmd_ctx->pathname, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->pathname);
		if (command != NULL)
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
