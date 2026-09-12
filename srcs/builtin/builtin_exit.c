#include "minishell.h"
#include <limits.h>

static int	ft_isnumber(char *str)
{
	while (*str && ft_isspace(*str))
	{
		str++;
	}
	if (*str == 0)
		return (0);
	if (*str && (*str == '-' || *str == '+'))
	{
		str++;
	}
	if (*str == 0)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static int exit_with_code(t_ctx *c, t_command_ctx *command_ctx)
{
	long int exit_code;

	if (ft_isnumber(command_ctx->argv[1]))
	{
		exit_code = ft_atol(command_ctx->argv[1]);
		if (exit_code < INT_MAX && exit_code > INT_MIN)
		{
			cleanup_shell(c, command_ctx, NULL);
			exit(exit_code);
		}
	}
	msh_error("exit", command_ctx->argv[1], "numeric argument required");
	c->should_exit = true;
	return (2);
}

int	builtin_exit(t_ctx *c, t_command_ctx *command_ctx)
{
	if (command_ctx->argc > 2)
	{
		msh_error("exit", NULL, "too many arguments");
		return (2);
	}
	else if (command_ctx->argc == 2)
	{
		return (exit_with_code(c, command_ctx));
	}
	c->should_exit = true;
	return (EXIT_SUCCESS);
}
