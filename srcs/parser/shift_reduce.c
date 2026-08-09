#include "minishell.h"

static t_lalr_action	reduce(t_ctx *c, t_parser_state *parse, int32_t action)
{
	t_rule		rule;
	t_symbol	sym;
#ifdef DEBUG
	char		rule_desc[256];
#endif

	rule = get_rule(action);
#ifdef DEBUG
	build_rule_desc(rule_desc, sizeof(rule_desc), action, c, &rule, parse);
#endif
	sym.node_idx = reduce_compute_result(c, parse, &rule, &sym.token_idx);
	sym.type = rule.lhs_type;
	parse->exec_root_idx = get_exec_root(c, parse, &rule);
	pop_symbols(c, parse, rule.rhs_len);
	reduce_apply_goto(parse, &rule);
#ifdef DEBUG
	if (c->dbg.states & DBG_PARSER)
	{
		if (sym.node_idx)
			print_node_line(stderr, c, get_node_from_idx(c, sym.node_idx),
				sym.node_idx);
	}
#endif
	push_symbol(c, parse, &sym);
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

static t_lalr_action	reduce_or_error(t_ctx *c, t_parser_state *parse,
		int32_t action)
{
	if (!action)
		return (LALR_ERROR);
	return (reduce(c, parse, action));
}

static t_lalr_action	shift(t_ctx *c, t_parser_state *parse, int32_t action)
{
	t_symbol	sym;

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
	sym.type = parse->lookahead_type;
	sym.token_idx = parse->token_idx;
	sym.node_idx = 0;
	push_symbol(c, parse, &sym);
	return (LALR_SHIFT);
}

t_lalr_action	shift_reduce(t_ctx *c, t_parser_state *parse)
{
	int32_t		action;
	int32_t		index;

	index = get_yypact(parse->state);
	if (index == YYPACT_NINF)
		return (reduce_or_error(c, parse, get_yydefact(parse->state)));
	index += parse->lookahead_type;
	if (index < 0 || index > YYLAST
		|| get_yycheck(index) != (int32_t)parse->lookahead_type)
		return (reduce_or_error(c, parse, get_yydefact(parse->state)));
	action = get_yytable(index);
	if (action <= 0)
		return (reduce_or_error(c, parse, -action));
	return (shift(c, parse, action));
}
