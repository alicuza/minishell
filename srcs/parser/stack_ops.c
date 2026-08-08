#include "minishell.h"
#include "parser.h"

void	push_term(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;
	t_arena		*stack;
	t_arena		*tokens;
	t_token		*token;

	stack = &(c->arena[AT_STACK]);
	tokens = &(c->arena[AT_TOKENS]);
	parse->stack_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	token = get_ptr_from_idx(tokens, parse->token_idx);
	symbol->token_idx = parse->token_idx;
	symbol->type = classify_token(c, token);
	symbol->entry_state = parse->state;
	symbol->node_idx = 0;
}

void	push_nonterm(t_ctx *c, t_parser_state *parse, t_symbol_type type,
		uint64_t node_idx, uint64_t token_idx)
{
	t_symbol	*symbol;
	t_arena		*stack;

	stack = &(c->arena[AT_STACK]);
	parse->stack_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	symbol->token_idx = token_idx;
	symbol->type = type;
	symbol->entry_state = parse->state;
	symbol->node_idx = node_idx;
}

static void	pop(t_ctx *c, t_parser_state *parse, uint32_t len)
{
	t_arena		*stack;
	t_symbol	*symbol;

	stack = &(c->arena[AT_STACK]);
	parse->stack_idx -= len;
	stack->offset -= len * stack->stride;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	parse->state = symbol->entry_state;
}

void	reduce_apply_goto(t_ctx *c, t_parser_state *parse, t_rule *rule)
{
	int32_t	lhs;
	int32_t	index;

	pop(c, parse, rule->rhs_len);
	lhs = rule->lhs_type - NTERM_OFFSET;
	index = get_yypgoto(lhs) + parse->state;
	if (0 <= index && index <= YYLAST && get_yycheck(index) == parse->state)
		parse->state = get_yytable(index);
	else
		parse->state = get_yydefgoto(lhs);
}
