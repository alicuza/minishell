/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:30:16 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/09 17:30:18 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finish_args(t_ctx *c, t_command_ctx *command)
{
	t_arena		*arguments;
	t_argv_slot	*slot;
	uint64_t	i;
	uint64_t	first;

	arguments = &c->arena[AT_ARGV];
	slot = get_ptr_from_offset(arguments, arena_alloc(arguments,
				sizeof(t_argv_slot), _Alignof(t_argv_slot)));
	slot->ptr = NULL;
	first = get_idx_from_offset(arguments, arguments->stride);
	i = 0;
	while (i < command->argc)
	{
		slot = get_ptr_from_idx(arguments, first + i);
		slot->ptr = (char *)get_ptr_from_offset(
				&c->arena[slot->ref.arena], slot->ref.offset);
		i++;
	}
	command->argv = (char **)get_ptr_from_idx(arguments, first);
}

void	expand_args_arena(t_ctx *c, t_command_ctx *command,
		t_expand_state *exp, t_node *arg_node)
{
	t_node	*node;
	char	*word;

	node = arg_node;
	while (node->type == NODE_ARG)
	{
		word = get_ptr_from_offset(&c->arena[AT_STRING],
				node->data.arg.arena_offset);
		if (!(node->data.arg.flags & (TKN_HAS_QUOTES | TKN_HAS_EXPANSION)))
			record_reference(c, command, AT_STRING,
				(uint32_t)node->data.arg.arena_offset);
		else
			scan_word(c, command, exp, word);
		node = get_ptr_from_idx(&c->arena[AT_COMMAND], node->next_idx);
	}
}

static void	resolve_redir_field(t_ctx *c, t_node *redir_node,
		size_t redir_base)
{
	t_arena		*arguments;
	t_argv_slot	*slot;

	arguments = &c->arena[AT_ARGV];
	slot = get_ptr_from_offset(&c->arena[AT_ARGV], redir_base);
	redir_node->data.redir.arena_offset = slot->ref.offset;
	arena_restore(arguments, redir_base);
}

static bool	redir_needs_scan(t_ctx *c, t_node *redir_node,
		t_expand_state *exp, char **word)
{
	if (redir_node->flags & REDIR_HERE)
		return (false);
	ft_memset(exp, 0, sizeof(*exp));
	*word = get_ptr_from_offset(&c->arena[AT_STRING],
			redir_node->data.redir.arena_offset);
	return (redir_node->data.redir.flags
		& (TKN_HAS_QUOTES | TKN_HAS_EXPANSION));
}

int	expand_redir_arena(t_ctx *c, t_node *redir_node, t_expand_state *exp)
{
	t_command_ctx	tmp;
	size_t			redir_base;
	char			*word;

	if (!redir_needs_scan(c, redir_node, exp, &word))
		return (EXIT_SUCCESS);
	ft_memset(&tmp, 0, sizeof(tmp));
	redir_base = arena_save(&c->arena[AT_ARGV]);
	scan_word(c, &tmp, exp, word);
	if (tmp.argc != 1)
	{
		arena_restore(&c->arena[AT_ARGV], redir_base);
		return (EXIT_FAILURE);
	}
	resolve_redir_field(c, redir_node, redir_base);
	return (EXIT_SUCCESS);
}
