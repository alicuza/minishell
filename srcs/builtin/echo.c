#include "minishell.h"

#define EQUAL 0


static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static void	print_val(char *str, bool is_print_nl)
{
	if (is_print_nl)
	{
		if (is_empty(str))
			printf("\n");
		else
			printf("%s\n", str);
	}
	else if (!is_empty(str))
		printf("%s", str);
}

static int64_t	find_n_flag(t_command_ctx *command_ctx)
{
	int64_t i;
	int64_t j;
	int64_t found_at;

	found_at = -1;
	i = 1;
	j = 1;
	while (i < command_ctx->argc && command_ctx->argv[i][0] == '-')
	{
		while (command_ctx->argv[i][j] != '\0'
			&& command_ctx->argv[i][j] != 'n')
		{
			j++;
		}
		if (command_ctx->argv[i][j] == 'n')
			found = i;
		j = 1;
		i++;
	}
	return (found_at);
}

static bool	is_print_nl(uint64_t argc, int64_t i, int64_t n_flag_found_at)
{
	if (n_flag_found_at != -1)
		return (false);
	else
		return (i + 1 == argc);
}

int	echo(t_ctx *c, t_command_ctx *command_ctx)
{
	int64_t i;
	int64_t n_flag_found_at;
	char *str;
	char *tmp;

	if (command_ctx->argc == 1)
		print_val("");
	else
	{
		n_flag_found_at = find_n_flag(command_ctx);
		if (n_flag_found_at != -1)
			i = n_flag_found_at;
		else
			i = 1;
		while (i < command_ctx->argc)
		{
			print_val(command_ctx->argv[i], is_print_nl(command_ctx->argc, i,
					n_flag_found_at));
			if (i + 1 < command_ctx->argc)
				printf(" ");
			i++;
		}
	}
	return (0);
}
