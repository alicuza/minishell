/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:19 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/24 11:20:20 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	try_reduce_symbol(t_ctx *c, t_parser_state *parse)
{
	t_arena			*symbols;
	t_symbol		*symbol;
	//t_here_state	here;

	symbols = &c->arena[AT_STACK];
	symbol = get_ptr_from_offset(symbols, symbols->offset - symbols->stride + 1);
	if ((symbol - 1)->type == SYM_DLESS && symbol->type == SYM_WORD)
	{
		parse->flags |= PARSE_SAVE_TOKENS;
	}
}

t_parser_state	parse_input(t_ctx *c)
{
	t_parser_state	parse;
	t_lexer_state	lex;
	t_here_state	here;

	ft_memset(&lex, 0, sizeof(t_lexer_state));
	ft_memset(&parse, 0, sizeof(t_parser_state));
	arena_clear(&c->arena[AT_STRING]);
	arena_clear(&c->arena[AT_TOKENS]);
	arena_clear(&c->arena[AT_STACK]);
	while (get_next_token(c, &parse, &lex))
	{
#ifdef DEBUG
		FILE *out = stderr;
		if (c->scope & SCOPE_TOKENS)
			out = stdout;
		fprintf(stderr, "\n--- lookahead ---\n");
		print_token(out, c, get_ptr_from_idx(&c->arena[AT_TOKENS], parse.token_idx));
		print_arena(&c->arena[AT_STRING]);
		print_arena(&c->arena[AT_TOKENS]);
#endif
		if (parse.flags & PARSE_SAVE_TOKENS)
		{
			t_arena *strings = &c->arena[AT_STRING];
			t_token *cur_token = get_ptr_from_idx(&c->arena[AT_TOKENS], parse.token_idx);
			if (cur_token->type == TKN_OPERATOR && strings->buf[cur_token->offset] == '\n')
			{
				parse.flags &= ~PARSE_SAVE_TOKENS;
				parse.flags |= ~PARSE_HERE_BODY;
			}
		}
		else
		{
			shift_symbol(c, &parse);
			try_reduce_symbol(c, &parse);
		}
		if (parse.flags & PARSE_HERE_BODY)
		{
			if (line_is_delim(c, &here))
				parse.flags &= ~PARSE_HERE_BODY;
		}
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
