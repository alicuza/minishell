/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 13:03:24 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/26 21:10:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delimit_lex_here(t_ctx *c, t_here_state *here)
{
	t_arena		*tokens;
	t_token		*token;

	tokens = &c->arena[AT_TOKENS];
	token = get_ptr_from_offset(tokens,
		arena_alloc(tokens, sizeof(t_token), _Alignof(t_token)));
	token->type = TKN_WORD;
	token->body.pos = here->body.pos;
	token->flags = TKN_IS_HERE_BODY;
}

// TODO: the t_slice here is created during reduction of io_here, not done yet
bool	is_delim_line(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	t_arena		*strings;
	char		*line;

	strings = &c->arena[AT_STRING];
	line = c->read_line + lex->char_idx;
	return (!ft_strncmp(line, strings->buf + here->delim.pos, here->delim.len)
		&& line[here->delim.len] == '\n');
}

void	append_to_here_body(t_ctx *c, t_lexer_state *lex, t_here_state *here, uint64_t len)
{
	t_arena	*strings;

	strings = &c->arena[AT_STRING];
	if (!here->body.len)
		here->body.pos = arena_strlcpy(strings, c->read_line + lex->char_idx, len + 2);
	else
		arena_strlcat(strings, c->read_line + lex->char_idx, len + 2);
	here->body.len += len + 1;
	grow_lex_token(lex, len + 1);
	consume_char(lex, len + 1);
}

bool	get_here_doc(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	uint64_t	len;

	while (true)
	{
		if (!c->read_line[lex->char_idx])
		{
			get_user_input(c, INPUT_CONTINUATION);
			if (!c->read_line)
			{
				printf("minishell: warning: here-document delimited by end-of-file\n"); // TODO: proper error handling here.
				return (false) ;
			}
			ft_memset(lex, 0, sizeof(t_lexer_state));
		}
		len = word_len(c->read_line + lex->char_idx, '\n');
		if (!is_delim_line(c, lex, here))
			append_to_here_body(c, lex, here, len);
		else
		{
			consume_char(lex, len + 1);
			delimit_lex_here(c, here);
			return (true);
		}
	}
}

bool	handle_here_body(t_ctx *c, t_parser_state *parse, t_lexer_state *lex, t_here_state *here)
{
	t_arena		*stack;
	t_symbol	*symbol;

	stack = &c->arena[AT_STACK];
	if (!get_here_doc(c, lex, here))
	{
		lex->flags &= ~LEX_HERE_BODY;
		return (false);
	}
	lex->flags &= ~LEX_HERE_BODY;
	lex->flags |= LEX_HAS_SAVED_TOKENS;
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	parse->lookahead.token_idx = symbol->token_idx;							// resetting parse.lookahead.token_idx to the last shifted token after getting heredoc body
	return (true);
}

bool	handle_saved_tokens(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	t_arena	*tokens;
	t_arena	*stack;
	t_token	*next;

	tokens = &c->arena[AT_TOKENS];
	stack = &c->arena[AT_STACK];
	next = get_ptr_from_idx(tokens, parse->lookahead.token_idx + 1);
	if (next->flags & TKN_IS_HERE_BODY) // TODO: figure out if this is the correct place to write the heredoc to a tmp file
	{
		lex->flags &= ~LEX_HAS_SAVED_TOKENS;
		++parse->lookahead.token_idx;
		return (false);
	}
	++parse->lookahead.token_idx;
	return (true);
}
