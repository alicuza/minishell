#include "minishell.h"
#include <limits.h>


static int	ft_isnumber(char *str)
{
	int i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == 0)
		return (0);
	if (str[i] && (str[i] == '-' || str[i] == '+'))
		i++;
	if (str[i] == 0 || ft_isspace(str[i]))
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if(str[i] == 0)
		return 1;
	else while (str[i] && ft_isspace(str[i]))
		i++;
	return (str[i] == 0);
}

static int	exit_with_code(t_ctx *c, t_command_ctx *command_ctx)
{
	long int exit_code;

	if (command_ctx->argc == 1)
		exit_code = c->return_status;
	else
		exit_code = ft_atol(command_ctx->argv[1]);
	cleanup_shell(c, command_ctx, NULL);
	exit(exit_code);
	return (exit_code);
}
static int	handle_numeric_argument_required(t_ctx *c,
		t_command_ctx *command_ctx)
{
	msh_error("exit", command_ctx->argv[1], "numeric argument required");
	c->should_exit = true;
	return (2);
}

static int	handle_too_many_arguments(void)
{
	msh_error("exit", NULL, "too many arguments");
	return (1);
}

int	builtin_exit(t_ctx *c, t_command_ctx *command_ctx)
{
	errno = 0;
	if (command_ctx->argc > 1 && (!ft_isnumber(command_ctx->argv[1])
			|| (ft_atol(command_ctx->argv[1]) == 0 && errno == EINVAL)))
		return (handle_numeric_argument_required(c, command_ctx));
	if (command_ctx->argc > 2)
		return (handle_too_many_arguments());
	else
		return (exit_with_code(c, command_ctx));
}
