#include "minishell.h"
#include "parser.h"

/* -------- LALR driver ------------------------------------------------------*/
static t_lalr_action	reduce_or_error(t_ctx *c, t_parser_state *parse,
		int32_t action)
{
	if (!action)
		return (LALR_ERROR);
	return (reduce(c, parse, action));
}

t_lalr_action	reduce(t_ctx *c, t_parser_state *parse, int32_t action)
{
	t_rule		rule;
	uint64_t	node_idx;
	uint64_t	token_idx;
#ifdef DEBUG
	char		rule_desc[256];
#endif

	rule = get_rule(action);
#ifdef DEBUG
	build_rule_desc(rule_desc, sizeof(rule_desc), action, c, &rule, parse);
#endif
	node_idx = reduce_compute_result(c, parse, &rule, &token_idx);
	reduce_set_exec_idx(c, parse, &rule);
	reduce_apply_goto(c, parse, &rule);
#ifdef DEBUG
	if (c->dbg.states & DBG_PARSER)
	{
		if (node_idx)
			print_node_line(stderr, c, get_node_from_idx(c, node_idx),
				node_idx);
	}
#endif
	push_nonterm(c, parse, rule.lhs_type, node_idx, token_idx);
#ifdef DEBUG
	print_trace_step(c, parse, rule_desc);
#endif
	if (parse->state == YYFINAL)
	{
#ifdef DEBUG
		print_trace_step(c, parse, "accept");
#endif
		return (LALR_ACCEPT);
	}
	return (LALR_REDUCE);
}

static t_lalr_action	shift(t_ctx *c, t_parser_state *parse, int32_t action)
{
	parse->state = action;
#ifdef DEBUG
	debug_trace_shift(c, parse);
#endif
	if (parse->state == YYFINAL)
	{
#ifdef DEBUG
		print_trace_step(c, parse, "accept");
#endif
		return (LALR_ACCEPT);
	}
	push_term(c, parse);
	return (LALR_SHIFT);
}

t_lalr_action	shift_reduce(t_ctx *c, t_parser_state *parse)
{
	t_arena		*tokens;
	t_token		*token;
	int32_t		action;
	int32_t		index;
	int32_t		lookahead;

	tokens = &(c->arena[AT_TOKENS]);
	index = get_yypact(parse->state);
	if (index == YYPACT_NINF)
		return (reduce_or_error(c, parse, get_yydefact(parse->state)));
	if (parse->flags & PARSE_LOOKAHEAD_IS_EOF)
		lookahead = SYM_EOF;
	else
	{
		token = get_ptr_from_idx(tokens, parse->token_idx);
		lookahead = classify_token(c, token);
	}
	index += lookahead;
	if (index < 0 || index > YYLAST || get_yycheck(index) != lookahead)
		return (reduce_or_error(c, parse, get_yydefact(parse->state)));
	action = get_yytable(index);
	if (action <= 0)
		return (reduce_or_error(c, parse, -action));
	return (shift(c, parse, action));
}
