/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:03:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/23 20:26:08 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_lex_token(t_lexer_state *lex, t_token_type type)
{
	lex->token.pos = lex->char_idx;
	lex->token.len = 1;
	lex->type = type;
	lex->flags = LEX_IS_BUILDING;
}

void	delimit_lex_token(t_ctx *c, t_lexer_state *lex)
{
	t_arena	*strings;
	t_arena	*tokens;
	t_token	*token;

	strings = &c->arena[AT_STRING];
	tokens = &c->arena[AT_TOKENS];
	token = get_ptr_from_offset(tokens,
		arena_alloc(tokens, sizeof(t_token), _Alignof(t_token)));
	token->type = lex->type;
	lex->type = TKN_NONE;
	token->offset =
		arena_strlcpy(strings, c->read_line + lex->token.pos, lex->token.len + 1);
	token->flags = lex->flags & ~(LEX_IS_BUILDING | LEX_AT_EOI);
	lex->flags &= ~LEX_IS_BUILDING;
}

uint64_t	grow_lex_token(t_lexer_state *lex, uint64_t len)
{
	lex->token.len += len;
	return (lex->token.len);
}
