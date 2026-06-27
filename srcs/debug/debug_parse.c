/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/27 17:14:31 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_symbol(t_ctx *c, t_symbol *symbol, uint64_t phys)
{
	t_arena	*input;

	input = &(c->arena[AT_STRING]);
	fprintf(stderr, "\n--- symbol ---\n");
	fprintf(stderr, "  [%lu]  %s(", phys,
		get_symbol_type_name(symbol->type));
	print_escaped_str(stderr, input->buf + symbol->offset);
	fprintf(stderr, ")  {  offset = %lu"
		"  state = %u  prev = %lu  flags = ",
		symbol->offset, symbol->entry_state,
		symbol->prev_symbol);
	print_flags(stderr, symbol->flags);
	fprintf(stderr, "  }\n");
}

static void	print_stack_entry(t_ctx *c, t_symbol *symbol,
	uint64_t phys, uint64_t prev)
{
	t_arena	*input;

	input = &c->arena[AT_STRING];
	fprintf(stderr, "  [%lu] [%lu]  %s(", phys, prev,
		get_symbol_type_name(symbol->type));
	print_escaped_str(stderr, input->buf + symbol->offset);
	fprintf(stderr, ")  {  offset = %lu"
		"  state = %u  prev = %lu  flags = ",
		symbol->offset, symbol->entry_state,
		symbol->prev_symbol);
	print_flags(stderr, symbol->flags);
	fprintf(stderr, "  }\n");
}

void	print_complete_stack(t_ctx *c, t_parser_state *parse)
{
	uint64_t		phys;
	t_symbol		*symbol;
	t_arena			*stack;

	stack = &c->arena[AT_STACK];
	fprintf(stderr, "\n--- complete stack ---\n");
	fprintf(stderr, "--- top -------------\n");
	phys = parse->arena_idx;
	while (phys)
	{
		symbol = get_symbol_from_idx(stack, phys);
		print_stack_entry(c, symbol, phys, symbol->prev_symbol);
		--phys;
	}
	fprintf(stderr, "--- bottom ----------\n");
}
