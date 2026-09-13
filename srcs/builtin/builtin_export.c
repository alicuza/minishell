/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:10:39 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:10:42 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

#define EQUAL 0

static void	print_val(void *content_void_p)
{
	t_env_content	*content;

	content = (t_env_content *)content_void_p;
	if (content->val == NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd((char *)content->key, STDOUT_FILENO);
	}
	else
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd((char *)content->key, STDOUT_FILENO);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd((char *)content->val, STDOUT_FILENO);
		ft_putendl_fd("\"", STDOUT_FILENO);
	}
}

int	add_args_to_env(t_ctx *c, t_command_ctx *command_ctx)
{
	char		**tmp;
	uint64_t	i;
	int			result;

	i = 1;
	result = EXIT_SUCCESS;
	while (i < command_ctx->argc)
	{
		tmp = ft_split_key_value(command_ctx->argv[i], '=');
		if (tmp == NULL)
			handle_mem_error(c, command_ctx);
		if (is_valid_var_name(tmp[0]) == false)
		{
			msh_error("export", command_ctx->argv[i], "not a valid identifier");
			result = EXIT_FAILURE;
			free_str_arr(tmp);
			i++;
			continue ;
		}
		if (env_update(&c->env, tmp[0], tmp[1]))
			return (free(tmp), EXIT_FAILURE);
		free(tmp);
		i++;
	}
	return (result);
}

int	builtin_export(t_ctx *c, t_command_ctx *command_ctx)
{
	if (command_ctx->argc == 1)
	{
		ft_lstiter(c->env.vals, &print_val);
		return (EXIT_SUCCESS);
	}
	else
		return (add_args_to_env(c, command_ctx));
}
