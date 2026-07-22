/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:19 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/22 10:54:50 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser_state	parse_input(t_ctx *c)
{
	t_parser_state	parse;
	t_lexer_state	lex;

	ft_memset(&lex, 0, sizeof(t_lexer_state));
	ft_memset(&parse, 0, sizeof(t_parser_state));
	arena_clear(&c->arena[AT_STRING]);
	arena_clear(&c->arena[AT_TOKENS]);
	arena_clear(&c->arena[AT_STACK]);
	while (true)
	{
		if (!get_lookahead(c, &lex))
			break ;
		++parse.token_idx;
#ifdef DEBUG
		FILE *out = stderr;
		if (c->scope & SCOPE_TOKENS)
			out = stdout;
		fprintf(stderr, "\n--- lookahead ---\n");
		print_token(out, c, get_ptr_from_idx(&c->arena[AT_TOKENS], parse.token_idx));
		print_arena(&c->arena[AT_STRING]);
		print_arena(&c->arena[AT_TOKENS]);
#endif
		shift_symbol(c, &parse);
#ifdef DEBUG
		{
			t_symbol *symbol;
			symbol = get_ptr_from_idx(&c->arena[AT_STACK], parse.stack_idx);
			print_symbol(c, symbol, parse.stack_idx);
			print_arena(&c->arena[AT_STACK]);
		}
#endif
	}
	return (parse);
}
