/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/21 19:59:39 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/22 09:42:12 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	matching_close(char open)
{
	char	close;

	close = open;
	if (open == '(')
		close = ')';
	return (close);
}

uint64_t	find_matched_pair(t_ctx *c, t_lexer_state *lex)
{
    t_slice	save;

    save = save_lex_token_slice(lex);
    consume_char(lex, 1);
    while (c->read_line[lex->char_idx]
		&& c->read_line[lex->char_idx] != lex->pair.close)
    {
		if (lex->pair.open == '"' && is_expansion_start(c->read_line, lex->char_idx))
			lex->flags |= TKN_HAS_EXPANSION;
        grow_lex_token(lex, 1);
        consume_char(lex, 1);
    }
    if (!c->read_line[lex->char_idx])
    {
		restore_lex_token_slice(lex, save);
        return (lex->char_idx);
    }
    grow_lex_token(lex, 1);
	lex->flags |= TKN_HAS_QUOTES;
    return (lex->char_idx);
}
