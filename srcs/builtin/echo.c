#include "minishell.h"

#define EQUAL 0

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static void	print_val(char *str)
{
	if (is_empty(str))
		printf("\n");
	else
		printf("%s\n", str);
}

static char	*get_exit_status(t_ctx *c)
{
	return (ft_itoa(c->return_status));
}

//TODO add proper arg processing with white space and -n flag see 2.6.5 Field Splitting
int	echo(t_ctx *c, t_command_ctx *command_ctx)
{
	char *str;
	char *tmp;

	if (command_ctx->argc == 1)
		str = "";
	else
		str = command_ctx->argv[1];
	if (str[0] == '$')
	{
		if (ft_strncmp(str + 1, "?", 2) == EQUAL)
			tmp = get_exit_status(c);
		else
			tmp = env_get(&c->env, str + 1);
		print_val(tmp);
		free(tmp);
		return (0);
	}
	print_val(str);
	return (0);
}
