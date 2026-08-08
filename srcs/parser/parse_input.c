/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    ft_t         :+:   :+:   */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  :+:       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:19 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/08 14:37:26 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	parse_exec(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;

	if (!parse->exec_idx)
		return ;
#ifndef DEBUG
	exec_list(c, parse->exec_idx);
#else
	if (!c->dbg.no_exec)
		exec_list(c, parse->exec_idx);
#endif
	parse->exec_idx = 0;
#ifdef DEBUG
	if (c->dbg.no_exec)
		return ;
#endif
	arena_clear(&c->arena[AT_COMMAND]);
	symbol = get_ptr_from_idx(&c->arena[AT_STACK], parse->stack_idx);
	symbol->node_idx = 0;
}

static bool	parse_advance(t_ctx *c, t_parser_state *parse,
		t_lalr_action action)
{
	if (action == LALR_REDUCE)
		return (true);
	parse->flags &= ~PARSE_HAS_LOOKAHEAD;
	if (action == LALR_SHIFT)
		return (true);
	if (action == LALR_ACCEPT)
	{
		parse->flags |= PARSE_DONE;
		return (false);
	}
	report_parse_error(c, parse);
	return (false);
}

static bool	run_parse_iteration(t_ctx *c, t_parser_state *parse,
		t_lexer_state *lex)
{
	t_lalr_action	action;

#ifdef DEBUG
	if (c->dbg.states & DBG_PARSER)
		debug_parse_header(parse);
	debug_parse_arenas(c);
#endif
	if (!(parse->flags & PARSE_HAS_LOOKAHEAD))
	{
		if (!get_lookahead(c, parse, lex))
			return (false);
	}
	if (parse->flags & PARSE_SAVE_TOKENS)
	{
		parse->flags &= ~PARSE_HAS_LOOKAHEAD;
		return (handle_here_doc(c, parse));
	}
	action = shift_reduce(c, parse);
	parse_exec(c, parse);
#ifdef DEBUG
	debug_parse_action(c, parse, action);
#endif
	return (parse_advance(c, parse, action));
}

static void	final_pass(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	t_lalr_action	action;

	(void)lex;
	while (true)
	{
		action = shift_reduce(c, parse);
		parse_exec(c, parse);
		if (action == LALR_ACCEPT)
		{
			parse->flags |= PARSE_DONE;
			return ;
		}
		if (action == LALR_ERROR)
		{
			report_parse_error(c, parse);
			return ;
		}
	}
}

t_parser_state	parse_input(t_ctx *c)
{
	t_parser_state	parse;
	t_lexer_state	lex;

	ft_memset(&parse, 0, sizeof(t_parser_state));
	ft_memset(&lex, 0, sizeof(t_lexer_state));
	arena_clear(&c->arena[AT_STRING]);
	arena_clear(&c->arena[AT_TOKENS]);
	arena_clear(&c->arena[AT_STACK]);
	arena_clear(&c->arena[AT_COMMAND]);
	while (run_parse_iteration(c, &parse, &lex))
		;
	if (!(parse.flags & PARSE_ERROR) && !(parse.flags & PARSE_DONE))
	{
		parse.flags |= PARSE_LOOKAHEAD_IS_EOF;
		final_pass(c, &parse, &lex);
	}
	if (!(parse.flags & PARSE_ERROR) && (parse.flags & PARSE_SAVE_TOKENS))
		get_here_doc(c, &lex, &parse.here);
	return (parse);
}