/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 13:39:18 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 17:00:24 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint64_t	try_as_quote_pair(t_ctx *c, t_lexer_state *lex)
{
    t_slice	save;
    char	quote;

    save = save_lex_token_slice(lex);
    quote = c->read_line[lex->char_idx];
    consume_char(lex);
    while (c->read_line[lex->char_idx] && c->read_line[lex->char_idx] != quote)
    {
        grow_lex_token(lex);
        consume_char(lex);
    }
    if (!c->read_line[lex->char_idx])
    {
		restore_lex_token_slice(lex, save);
        return (lex->char_idx);
    }
    grow_lex_token(lex);
	lex->flags |= LEX_HAS_QUOTES;
    return (lex->char_idx);
}
