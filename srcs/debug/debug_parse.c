/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/22 10:36:13 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_symbol(t_ctx *c, t_symbol *symbol, uint64_t idx)
{
	t_arena	*input;
	t_arena	*tokens;
	t_token	*token;

	tokens = &c->arena[AT_TOKENS];
	input = &c->arena[AT_STRING];
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	fprintf(stderr, "\n--- symbol ---\n");
	fprintf(stderr, "  [%lu]  %s(", idx, get_symbol_type_name(symbol->type));
	print_escaped_str(stderr, input->buf + token->body.pos);
	fprintf(stderr, ")  {  token_idx = %lu state = %u  node_idx = %lu  flags = ",
		symbol->token_idx, symbol->entry_state, symbol->node_idx);
	print_flags(stderr, token->flags);
	fprintf(stderr, "  }\n");
}

void	print_complete_stack(t_ctx *c, t_parser_state *parse)
{
	uint64_t	phys;
	t_symbol		*symbol;
	t_arena		*stack;

	stack = &c->arena[AT_STACK];
	fprintf(stderr, "\n--- complete stack ---\n");
	fprintf(stderr, "--- top -------------\n");
	phys = parse->stack_idx;
	while (phys)
	{
		symbol = get_ptr_from_idx(stack, phys);
		print_symbol(c, symbol, phys);
		--phys;
	}
	fprintf(stderr, "--- bottom ----------\n");
}
