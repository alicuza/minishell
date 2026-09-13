/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_search_and_execution.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 01:48:49 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/14 01:48:49 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

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

static int	handle_failed_orig_name(t_ctx *c, t_command_ctx *cmd_ctx)
{
	t_error	e;

	e = (t_error){NULL, strerror(ENOMEM), 1};
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = NULL;
	msh_exit(c, cmd_ctx, &e, NULL);
	return (EXIT_FAILURE);
}

static int	handle_command_not_found(t_ctx *c, char *orig_name,
		t_node *redir_node)
{
	if (process_redirection(c, redir_node) == EXIT_FAILURE)
		return (free(orig_name), close_io(c), 1);
	close_io(c);
	if (orig_name)
		msh_error(NULL, orig_name, "command not found");
	else
		msh_error(NULL, "", "command not found");
	free(orig_name);
	return (127);
}

static int	search_and_execute_non_builtin(t_ctx *c, t_command_ctx *cmd_ctx,
		t_node *redir_node)
{
	char				*orig_name;
	int					status;

	orig_name = ft_strdup(cmd_ctx->pathname);
	if (orig_name == NULL)
		return (handle_failed_orig_name(c, cmd_ctx));
	status = get_pathname(c, cmd_ctx);
	if (status == EXIT_FAILURE)
	{
		handle_mem_error(c, cmd_ctx, orig_name);
		return (EXIT_FAILURE);
	}
	else if (status == EXIT_SUCCESS && cmd_ctx->pathname != NULL)
	{
		free(orig_name);
		return (execute_non_builtin(c, cmd_ctx, redir_node));
	}
	else
		return (handle_command_not_found(c, orig_name, redir_node));
}

int	command_search_and_execution(t_ctx *c, t_command_ctx *cmd_ctx,
		t_node *redir_node)
{
	t_command_function	command;

	if (ft_strchr(cmd_ctx->pathname, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->pathname);
		if (command != NULL)
			return (execute_builtin(c, cmd_ctx, command, redir_node));
		else
			return (search_and_execute_non_builtin(c, cmd_ctx, redir_node));
	}
	else
		return (execute_non_builtin(c, cmd_ctx, redir_node));
}
