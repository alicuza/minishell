/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:30:48 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/11 12:45:35 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_command(t_ctx *c, t_command_ctx *command, t_expand_state *exp)
{
	ft_memset(command, 0, sizeof(*command));
	ft_memset(exp, 0, sizeof(*exp));
	arena_reset(&c->arena[AT_FIELDS]);
	arena_reset(&c->arena[AT_ARGV]);
}

static int	set_command_pathname(t_ctx *c, t_command_ctx *command)
{
	t_error	e;

	if (command->argc == 0)
		return (EXIT_SUCCESS);
	if (is_empty_str(command->argv[0]))
		return (EXIT_SUCCESS);
	command->pathname = ft_strdup(command->argv[0]);
	if (command->pathname == NULL)
	{
		e = (t_error){NULL, strerror(ENOMEM), 1};
		msh_exit(c, command, &e, NULL);
	}
	return (EXIT_SUCCESS);
}

int	build_command(t_ctx *c, t_command_ctx *command,
		t_node *arg_node, t_node *redir_node)
{
	t_expand_state	exp;

	init_command(c, command, &exp);
	expand_args(c, command, &exp, arg_node);
	while (redir_node->type == NODE_REDIR)
	{
		if (expand_redir(c, redir_node, &exp) == EXIT_FAILURE)
		{
			char	*word;

			if (redir_node->data.redir.flags
				& (TKN_HAS_QUOTES | TKN_HAS_EXPANSION))
				word = get_ptr_from_offset(&c->arena[AT_FIELDS],
						redir_node->data.redir.arena_offset);
			else
				word = get_ptr_from_offset(&c->arena[AT_STRING],
						redir_node->data.redir.arena_offset);
			msh_error(NULL, word, "ambiguous redirect");
			return (EXIT_FAILURE);
		}
		redir_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				redir_node->next_idx);
	}
	finish_args(c, command);
	return (set_command_pathname(c, command));
}
