/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/28 21:42:26 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shift_symbol(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;
	t_arena		*stack;

	stack = &(c->arena[AT_STACK]);
	parse->arena_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_symbol_from_idx(stack, parse->arena_idx);
	symbol->offset = parse->lookahead.offset;
	symbol->prev_symbol = parse->stack_idx;
	symbol->type = parse->lookahead.type;
	symbol->entry_state = parse->cur_state;
	symbol->flags = parse->lookahead.flags;
	parse->stack_idx = parse->arena_idx;
}
