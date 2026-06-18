/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:03:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 16:48:52 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint64_t	consume_char(t_lexer_state *lex)
{
	return (++(lex->char_idx));
}

t_slice	save_lex_token_slice(t_lexer_state *lex)
{
	t_slice	save;

	save.pos = lex->char_idx;
	save.len = lex->token.len;
	return (save);
}

void	restore_lex_token_slice(t_lexer_state *lex, t_slice saved)
{
	lex->char_idx = saved.pos;
	lex->token.len = saved.len;
}
