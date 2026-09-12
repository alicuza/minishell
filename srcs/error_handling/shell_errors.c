/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:14:35 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/12 17:04:58 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_error(char *where, char *what, char *why)
{
	ft_putstr_fd(SHELLNAME": ", STDERR_FILENO);
	if (where)
	{
		ft_putstr_fd(where, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (what)
	{
		ft_putstr_fd(what, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(why, STDERR_FILENO);
	return (1);
}

void	cleanup_context(void *ctx)
{
	t_ctx			*c;
	t_arena_type	i;

	c = (t_ctx *)ctx;
	i = -1;
	while (++i < AT_COUNT)
		arena_free(&c->arena[i]);
	free(c->read_line);
	c->read_line = NULL;
	rl_clear_history();
	free_env(&c->env);
	close_all_fds(c);
	get_next_line(-255);
}

void	cleanup_shell(t_ctx *c, t_command_ctx *cmd, char **envp)
{
	if (cmd)
	{
		free(cmd->pathname);
		cmd->pathname = NULL;
	}
	free_str_arr(envp);
	cleanup_context(c);
}

void	free_str_arr(char **val)
{
	int	i;

	if (!val)
		return ;
	i = 0;
	while (val[i] != NULL)
	{
		free(val[i]);
		i++;
	}
	free(val);
}

void	msh_exit(t_ctx *c, t_command_ctx *cmd, t_error *e, char **envp)
{
	msh_error(NULL, e->name, e->msg);
	cleanup_shell(c, cmd, envp);
	exit(e->status);
}
