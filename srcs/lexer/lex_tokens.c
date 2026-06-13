/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:03:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 14:04:41 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_lex_token(t_lexer_state *lex, t_symbol_type type)
{
	lex->token.pos = lex->char_idx;
	lex->token.len = 1;
	lex->type = type;
	lex->flags = LEX_IS_BUILDING;
}

t_token	delimit_lex_token(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	t_arena	*strings;
	t_token	token;

	(void)parse;
	strings = &c->arena[AT_STRING];
	token.type = lex->type;
	token.offset =
		arena_strlcpy(strings, c->read_line + lex->token.pos, lex->token.len + 1);
	lex->flags &= ~LEX_IS_BUILDING;
	token.flags = lex->flags;
	lex->flags |= LEX_IS_DELIMITED;
	lex->type = SYM_NONE;
	return (token);
}

uint64_t	grow_lex_token(t_lexer_state *lex)
{
	return (++(lex->token.len));
}
