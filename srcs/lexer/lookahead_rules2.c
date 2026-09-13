/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookahead_rules2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 19:47:46 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/13 19:47:48 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	apply_rule_6(t_ctx *c, t_lexer_state *lex)
{
	if (lex->flags & LEX_IS_BUILDING)
	{
		delimit_lex_token(c, lex);
		return (true);
	}
	start_lex_token(lex, TKN_OPERATOR);
	consume_char(lex, 1);
	return (false);
}

bool	apply_rule_7(t_ctx *c, t_lexer_state *lex)
{
	consume_char(lex, 1);
	if (lex->flags & LEX_IS_BUILDING)
	{
		delimit_lex_token(c, lex);
		return (true);
	}
	return (false);
}

bool	apply_rule_8(t_ctx *c, t_lexer_state *lex)
{
	(void)c;
	grow_lex_token(lex, 1);
	consume_char(lex, 1);
	return (false);
}

bool	apply_rule_9(t_ctx *c, t_lexer_state *lex)
{
	while (c->read_line[lex->char_idx] && c->read_line[lex->char_idx] != '\n')
		consume_char(lex, 1);
	return (false);
}

bool	apply_rule_10(t_ctx *c, t_lexer_state *lex)
{
	(void)c;
	start_lex_token(lex, TKN_WORD);
	consume_char(lex, 1);
	return (false);
}
