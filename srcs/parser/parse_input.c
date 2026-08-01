/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:19 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/26 23:09:03 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	try_reduce_symbol(t_ctx *c, t_parser_state *parse, t_lexer_state *lex, t_here_state *here)
{
	(void)parse;
	t_arena			*strings;
	t_arena			*stack;
	t_arena			*tokens;
	t_symbol		*symbol;
	t_token			*token;

	strings = &c->arena[AT_STRING];
	tokens = &c->arena[AT_TOKENS];
	stack = &c->arena[AT_STACK];
	symbol =
		get_ptr_from_offset(stack, stack->offset - stack->stride);
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	if ((symbol - 1)->type == SYM_DLESS && symbol->type == SYM_WORD)
	{
		lex->flags |= LEX_SAVE_TOKENS;
	    here->body.pos = 0;
    	here->body.len = 0;
		here->delim.pos = token->body.pos;
		here->delim.len = ft_strlen(get_ptr_from_offset(strings, here->delim.pos));
		if (token->flags & TKN_HAS_QUOTES)	// TODO: needs more roburs handling for quuotes in any position
		{
			here->delim.pos += 1;
			here->delim.len -= 2;
		}
	}
}

bool	get_next_token(t_ctx *c, t_parser_state *parse, t_lexer_state *lex, t_here_state *here)
{
	while (true)
	{
		if ((lex->flags & LEX_HERE_BODY)
			&& !handle_here_body(c, parse, lex, here))
			return (false);
		if (lex->flags & LEX_AT_EOI)
		{
			ft_memset(lex, 0, sizeof(t_lexer_state));
			return (false);													// eof reached
		}
		if (lex->flags & LEX_HAS_SAVED_TOKENS)
		{
			if (handle_saved_tokens(c, parse, lex))
				return (true);
//			return (false);
		}
		if (lex_token(c, lex))
		{
			++parse->lookahead.token_idx;
			return (true);
		}
	}
}

t_parser_state	parse_input(t_ctx *c)
{
	t_parser_state	parse;
	t_lexer_state	lex;
	t_here_state	here; // TODO: think about adding t_here_state to t_parser_state
	t_arena			*strings;
	t_arena			*tokens;
	t_arena			*stack;
	t_token			*cur_token;

	ft_memset(&lex, 0, sizeof(t_lexer_state));
	ft_memset(&parse, 0, sizeof(t_parser_state));
	ft_memset(&here, 0, sizeof(t_here_state));
	strings = &c->arena[AT_STRING];
	tokens = &c->arena[AT_TOKENS];
	stack = &c->arena[AT_STACK];
	arena_clear(strings);
	arena_clear(tokens);
	while (true)
	{
		if(!get_next_token(c, &parse, &lex, &here))
			break ;
		parse.lookahead = *(t_symbol *)get_ptr_from_offset(stack, stack->offset);
		cur_token = get_ptr_from_idx(tokens, parse.lookahead.token_idx);
#ifdef DEBUG
		FILE *out = stderr;
		if (c->scope & SCOPE_TOKENS)
			out = stdout;
		fprintf(stderr, "\n--- lookahead ---\n");
		print_token(out, c, cur_token);
		print_arena(&c->arena[AT_STRING]);
		print_arena(&c->arena[AT_TOKENS]);
#endif
		if (lex.flags & LEX_SAVE_TOKENS)
		{
			if (cur_token->type == TKN_OPERATOR
				&& strings->buf[cur_token->body.pos] == '\n')
			{
				lex.flags &= ~LEX_SAVE_TOKENS;
				lex.flags |= LEX_HERE_BODY;
			}
		}
		else
		{
			shift_symbol(c, &parse);
			try_reduce_symbol(c, &parse, &lex, &here);
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
