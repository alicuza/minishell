/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lookahead.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:37:40 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/27 23:37:34 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token get_lookahead(t_ctx *c, t_parser_state *p, t_lexer_state *l)
{
	uint64_t	len;
	t_token		token;

	token = (t_token){0};
	if (l->flags & LEX_NEEDS_INPUT)
	{
		ft_memset(l, 0, sizeof(t_lexer_state));
		return (token);
	}
	while (!(l->flags & LEX_NEEDS_INPUT))
	{
#ifdef DEBUG
		print_lex_state(c, l);
#endif
		if (!c->read_line[l->char_idx])										// rule 1
		{
#ifdef DEBUG
			fprintf(stderr, "rule 1\n");
#endif
			if (l->flags & LEX_IS_BUILDING)
        		token = delimit_lex_token(c, p, l);
			else
				l->flags &= ~LEX_IS_DELIMITED;
			l->flags |= LEX_NEEDS_INPUT;
			return (token);
		}
		else if ((l->type == SYM_OPERATOR)									// rule 2
			&& is_str_in_set(c->read_line + l->char_idx - 1, get_operator_strs()))
		{
#ifdef DEBUG
			fprintf(stderr, "rule 2\n");
#endif
			if(l->flags & LEX_IS_BUILDING)
			{
				consume_char(l);
				grow_lex_token(l);
				token = delimit_lex_token(c, p, l);
				return (token);
			}
		}
		else if ((l->type == SYM_OPERATOR)									// rule 3
			&& !is_str_in_set(c->read_line + l->char_idx - 1, get_operator_strs()))
		{
#ifdef DEBUG
			fprintf(stderr, "rule 3\n");
#endif
			token = delimit_lex_token(c, p, l);
			return (token);
		}
		else if (is_char_in_set(c->read_line[l->char_idx], QUOTE_SET))		// rule 4
		{
#ifdef DEBUG
			fprintf(stderr, "rule 4\n");
#endif
			if (l->flags & LEX_IS_BUILDING)
				grow_lex_token(l);
			else
				start_lex_token(l, SYM_TOKEN);
			l->char_idx = try_as_quote_pair(c, l) + 1;
		}
		else if (is_expansion_start(c->read_line, l->char_idx))				// rule 5
		{
#ifdef DEBUG
			fprintf(stderr, "rule 5\n");
#endif
			len = get_expansion_len(c->read_line + l->char_idx);
			if (!(l->flags & LEX_IS_BUILDING))
			{
				start_lex_token(l, SYM_TOKEN);
				--len;
			}
			l->token.len += len;
			l->char_idx += len + 1;
			l->flags |= LEX_HAS_EXPANSION;
		}
		else if (is_char_in_set(c->read_line[l->char_idx], OPERATOR_SET)	// rule 6
			&& (c->read_line[l->char_idx] != '&' || c->read_line[l->char_idx + 1] == '&'))
		{
#ifdef DEBUG
			fprintf(stderr, "rule 6\n");
#endif
			if (l->flags & LEX_IS_BUILDING)
			{
				token = delimit_lex_token(c, p, l);
				return (token);
			}
			start_lex_token(l, SYM_OPERATOR);
			consume_char(l);
		}
		else if (is_char_in_set(c->read_line[l->char_idx], BLANK_SET))		// rule 7
		{
#ifdef DEBUG
			fprintf(stderr, "rule 7\n");
#endif
			consume_char(l);
			if(l->flags & LEX_IS_BUILDING)
			{
				token = delimit_lex_token(c, p, l);
				return (token);
			}
		}
		else if (l->type == SYM_TOKEN)										// rule 8
		{
#ifdef DEBUG
			fprintf(stderr, "rule 8\n");
#endif
			grow_lex_token(l);
			consume_char(l);
		}
		else if (c->read_line[l->char_idx] == '#')		// rule 9
		{
#ifdef DEBUG
			fprintf(stderr, "rule 9\n");
#endif
			while (c->read_line[l->char_idx] && c->read_line[l->char_idx] != '\n')
				consume_char(l);
		}
		else		// rule 10
		{
#ifdef DEBUG
			fprintf(stderr, "rule 10\n");
#endif
			start_lex_token(l, SYM_TOKEN);
			consume_char(l);
		}
	}
	token.type = SYM_NONE;
	return (token);
}
