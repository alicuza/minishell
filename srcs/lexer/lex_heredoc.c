/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 13:03:24 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/10 19:42:52 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	here_body_has_expansion(t_ctx *c, t_here_state *here)
{
	uint64_t	i;
	char		*body;

	body = get_ptr_from_offset(&c->arena[AT_STRING], here->body.pos);
	i = 0;
	while (i < here->body.len)
	{
		if (is_expansion_start(body, i))
			return (true);
		++i;
	}
	return (false);
}

void	delimit_lex_here(t_ctx *c, t_here_state *here)
{
	t_token		*token;
	uint64_t	token_idx;

	token_idx = alloc_token(c);
	token = get_token_from_idx(c, token_idx);
	token->type = TKN_WORD;
	token->offset = here->body.pos;
	token->flags = TKN_IS_HERE_BODY;
	if (here_body_has_expansion(c, here))
		token->flags |= TKN_HAS_EXPANSION;
}

bool	handle_here_body(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	t_symbol	*symbol;
	t_node		*node;

#ifdef DEBUG
	if (c->dbg.states & DBG_HEREDOC)
		print_here_reading(c, parse);
#endif
	get_here_doc(c, lex, &parse->here);
	parse->flags &= ~PARSE_HERE_BODY;
	parse->flags |= PARSE_HAS_SAVED_TOKENS;
	symbol = get_symbol_from_top(c, 1);
	node = get_node_from_idx(c, symbol->node_idx);
	node->data.redir.arena_offset = parse->here.body.pos;
	parse->token_idx = symbol->token_idx;
#ifdef DEBUG
	if (c->dbg.states & DBG_HEREDOC)
		print_here_stored(parse, symbol->token_idx);
#endif
	return (true);
}

bool	handle_saved_tokens(t_ctx *c, t_parser_state *parse)
{
	t_arena	*tokens;
	t_token	*next;

	tokens = &c->arena[AT_TOKENS];
	next = get_ptr_from_idx(tokens, parse->token_idx + 1);
#ifdef DEBUG
	if (c->dbg.states & DBG_HEREDOC)
		print_here_replay(parse->token_idx + 1);
#endif
	if (next->flags & TKN_IS_HERE_BODY)
	{
		parse->flags &= ~PARSE_HAS_SAVED_TOKENS;
		++parse->token_idx;
		return (false);
	}
	++parse->token_idx;
	return (true);
}
