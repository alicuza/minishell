#include "minishell.h"
#include "parser.h"

uint64_t	reduce_compute_result(t_ctx *c, t_parser_state *parse,
		t_rule *rule, uint64_t *token_idx)
{
	uint64_t	node_idx;

	if (rule->rhs_len)
	{
		if (rule->handler)
			node_idx = rule->handler(c, parse, rule);
		else
			node_idx = get_symbol_from_rhs(c, parse, rule, 0)->node_idx;
		*token_idx = get_symbol_from_rhs(c, parse, rule,
			rule->rhs_len - 1)->token_idx;
#ifdef DEBUG
		if (c->dbg.states & DBG_PARSER)
			log_rhs_symbols(c, parse, rule);
#endif
	}
	else
	{
		node_idx = 0;
		*token_idx = parse->token_idx;
	}
	return (node_idx);
}

void	reduce_set_exec_idx(t_ctx *c, t_parser_state *parse, t_rule *rule)
{
	if (rule->lhs_type != SYM_COMPLETE_COMMANDS)
		return ;
	parse->exec_idx = get_symbol_from_rhs(c, parse, rule,
		rule->rhs_len - 1)->node_idx;
}
