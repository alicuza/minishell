/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/26 13:03:24 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/08 14:23:13 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"
#include "parser.h"

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
	t_arena		*tokens;
	t_token		*token;

	tokens = &c->arena[AT_TOKENS];
	token = get_ptr_from_offset(tokens,
			arena_alloc(tokens, sizeof(t_token), _Alignof(t_token)));
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
		fprintf(stderr, "--- heredoc ---\n\treading body until delimiter\n");
#endif
	get_here_doc(c, lex, &parse->here);
	parse->flags &= ~PARSE_HERE_BODY;
	parse->flags |= PARSE_HAS_SAVED_TOKENS;
	symbol = get_symbol_from_top(c, 1);
	node = get_node_from_idx(c, symbol->node_idx);
	node->data.redir.arena_offset = parse->here.body.pos;
	parse->token_idx = symbol->token_idx;
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
		fprintf(stderr, "--- heredoc ---\n\treplaying saved token %lu\n",
			parse->token_idx + 1);
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