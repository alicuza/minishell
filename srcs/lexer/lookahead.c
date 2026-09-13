/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookahead.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:37:40 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/13 19:50:34 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	lex_token(t_ctx *c, t_lexer_state *lex)
{
	if (!c->read_line[lex->char_idx])
		return (apply_rule_1(c, lex));
	else if (lex->type == TKN_OPERATOR)
	{
		if (is_str_in_set(c->read_line + lex->char_idx - 1, get_op_strs()))
			return (apply_rule_2(c, lex));
		else
			return (apply_rule_3(c, lex));
	}
	else if (is_char_in_set(c->read_line[lex->char_idx], QUOTE_SET))
		return (apply_rule_4(c, lex));
	else if (is_expansion_start(c->read_line, lex->char_idx))
		return (apply_rule_5(c, lex));
	else if (is_char_in_set(c->read_line[lex->char_idx], OPERATOR_SET)
		&& (c->read_line[lex->char_idx] != '&'
			|| c->read_line[lex->char_idx + 1] == '&'))
		return (apply_rule_6(c, lex));
	else if (is_char_in_set(c->read_line[lex->char_idx], BLANK_SET))
		return (apply_rule_7(c, lex));
	else if (lex->type == TKN_WORD)
		return (apply_rule_8(c, lex));
	else if (c->read_line[lex->char_idx] == '#')
		return (apply_rule_9(c, lex));
	else
		return (apply_rule_10(c, lex));
}
