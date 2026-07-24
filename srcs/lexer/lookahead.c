/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookahead.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:37:40 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/24 11:37:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	apply_rule_1(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 1\n");
#endif
	lex->flags |= LEX_AT_EOI;
	if (lex->flags & LEX_IS_BUILDING)
	{
		delimit_lex_token(c, lex);
		return (true);
	}
	return (false);
}

bool	apply_rule_2(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 2\n");
#endif
	if(lex->flags & LEX_IS_BUILDING)
	{
		consume_char(lex, 1);
		grow_lex_token(lex, 1);
		delimit_lex_token(c, lex);
		return (true);
	}
	return (false);
}

bool	apply_rule_3(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 3\n");
#endif
	delimit_lex_token(c, lex);
	return (true);
}

bool	apply_rule_4(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 4\n");
#endif
	if (lex->flags & LEX_IS_BUILDING)
		grow_lex_token(lex, 1);
	else
		start_lex_token(lex, TKN_WORD);
	lex->pair.open = c->read_line[lex->char_idx];
	lex->pair.close = matching_close(lex->pair.open);
	if (!find_matched_pair(c, lex))
	{
		lex->pair.open = 0;
		lex->pair.close = 0;
		consume_char(lex, 1);
	}
	return (false);
}

bool	apply_rule_5(t_ctx *c, t_lexer_state *lex)
{
	uint64_t	len;
#ifdef DEBUG
	fprintf(stderr, "rule 5\n");
#endif
	len = get_expansion_len(c->read_line + lex->char_idx);
	if (!(lex->flags & LEX_IS_BUILDING))
	{
		start_lex_token(lex, TKN_WORD);
		grow_lex_token(lex, len - 1);
	}
	else
		grow_lex_token(lex, len);
	consume_char(lex, len);
	lex->flags |= TKN_HAS_EXPANSION;
	return (false);
}

bool	apply_rule_6(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 6\n");
#endif
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
#ifdef DEBUG
	fprintf(stderr, "rule 7\n");
#endif
	consume_char(lex, 1);
	if(lex->flags & LEX_IS_BUILDING)
	{
		delimit_lex_token(c, lex);
		return (true);
	}
	return (false);
}

bool	apply_rule_8(t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 8\n");
#endif
	grow_lex_token(lex, 1);
	consume_char(lex, 1);
	return (false);
}

bool	apply_rule_9(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 9\n");
#endif
	while (c->read_line[lex->char_idx] && c->read_line[lex->char_idx] != '\n')
		consume_char(lex, 1);
	return (false);
}

bool	apply_rule_10(t_lexer_state *lex)
{
#ifdef DEBUG
	fprintf(stderr, "rule 10\n");
#endif
	start_lex_token(lex, TKN_WORD);
	consume_char(lex, 1);
	return (false);
}

bool	lex_token(t_ctx *c, t_lexer_state *lex)
{
#ifdef DEBUG
		print_lex_state(c, lex);
#endif
	if (!c->read_line[lex->char_idx])										// rule 1
		return (apply_rule_1(c, lex));
	else if (lex->type == TKN_OPERATOR)										// rule 2 & 3
	{
		if (is_str_in_set(c->read_line + lex->char_idx - 1, get_operator_strs()))
			return (apply_rule_2(c, lex));
		else
			return (apply_rule_3(c, lex));
	}
	else if (is_char_in_set(c->read_line[lex->char_idx], QUOTE_SET))		// rule 4
		return (apply_rule_4(c, lex));
	else if (is_expansion_start(c->read_line, lex->char_idx))				// rule 5
		return (apply_rule_5(c, lex));
	else if (is_char_in_set(c->read_line[lex->char_idx], OPERATOR_SET)		// rule 6
			&& (c->read_line[lex->char_idx] != '&'
			|| c->read_line[lex->char_idx + 1] == '&'))
		return (apply_rule_6(c, lex));
	else if (is_char_in_set(c->read_line[lex->char_idx], BLANK_SET))		// rule 7
		return (apply_rule_7(c, lex));
	else if (lex->type == TKN_WORD)											// rule 8
		return (apply_rule_8(lex));
	else if (c->read_line[lex->char_idx] == '#')							// rule 9
		return (apply_rule_9(c, lex));
	else																	// rule 10
		return (apply_rule_10(lex));
}

bool	get_next_token(t_ctx *c, t_parser_state *parse, t_lexer_state *lex) // TODO: modify to make this the dispatch function for lex with different flags, for normal parsing and here_doc
{
	while (true)
	{
		if (lex->flags & LEX_AT_EOI)
		{
			ft_memset(lex, 0, sizeof(t_lexer_state));
				return (false);												// eof reached
		}
		if (lex_token(c, lex))
		{
			++parse->token_idx;
			return (true);
		}
	}
}

void	save_tokens(t_ctx *c, t_lexer_state *lex)
{
	while (c->read_line[lex->char_idx] != '\n')
		lex_token(c, lex);
	lex_token(c, lex);
}

// TODO: the here_state is created during reduction of io_here, not done yet
bool	line_is_delim(t_ctx *c, t_here_state *here)
{
	t_arena		*strings;
	t_arena		*tokens;
	t_token		*line;
	t_token		*delim;
	uint64_t	delim_len;

	strings = &c->arena[AT_STRING];
	tokens = &c->arena[AT_TOKENS];
	line = (t_token *)get_ptr_from_offset(tokens, here->cur_line);
	delim = (t_token *)get_ptr_from_offset(tokens, here->delim_idx);
	delim_len = ft_strlen(strings->buf + delim->offset);
	if (!ft_strncmp(strings->buf + line->offset, strings->buf + delim->offset, delim_len)
		&& strings->buf[line->offset + delim_len + 1] == '\n')
		return (true);
	return (false);
}

bool	get_here_doc(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	t_token		*here_line;
	char		*here_line_body;
	uint64_t	len;

	start_lex_token(lex, TKN_WORD);
	here_line = get_ptr_from_offset(&c->arena[AT_TOKENS], here->cur_line);
	here_line_body = get_ptr_from_offset(&c->arena[AT_STRING], here_line->offset);
	len = ft_strlen(here_line_body);
	while (true)
	{
		if (!here_line_body)
			return (false);
		if (line_is_delim(c, here))
			return (true);
		if (!c->read_line[lex->char_idx])
			return (false);
		consume_char(lex, len);
		grow_lex_token(lex, len);
	}
}
