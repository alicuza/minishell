/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:19 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/28 05:33:06 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parser_state	parse_input(t_ctx *c)
{
	t_parser_state	parse;
	t_lexer_state	lex;

	ft_memset(&lex, 0, sizeof(t_lexer_state));
	ft_memset(&parse, 0, sizeof(t_parser_state));
	parse.arena_idx = 1;
	arena_clear(&c->arena[AT_STRING]);
	arena_clear(&c->arena[AT_STACK]);
	while (1)
	{
		parse.lookahead = get_lookahead(c, &parse, &lex);
		if (!(lex.flags & LEX_IS_DELIMITED))
			break;
#ifdef DEBUG
		FILE *out = stderr;
		if (c->scope & SCOPE_TOKENS)
			out = stdout;
		fprintf(stderr, "\n--- lookahead ---\n");
		print_token(out, c, &parse.lookahead);
		print_arena(&c->arena[AT_STRING]);
#endif
		shift_symbol(c, &parse);
#ifdef DEBUG
		{
			t_symbol *sym;
			sym = get_symbol_from_idx(&c->arena[AT_STACK], parse.stack_idx);
			print_symbol(c, sym, parse.stack_idx);
			print_arena(&c->arena[AT_STACK]);
		}
#endif
	}
	return (parse);
}
