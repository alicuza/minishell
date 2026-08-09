#include "minishell.h"
#include "parser.h"

t_symbol	*get_symbol_from_rhs(t_ctx *c, t_parser_state *parse, t_rule *rule,
		uint32_t rhs_pos)
{
	t_arena	*stack;

	(void)parse;
	stack = &c->arena[AT_STACK];
	return (get_ptr_from_offset(stack,
			stack->offset - (rule->rhs_len - rhs_pos) * stack->stride));
}

void	*get_ptr_from_top(t_arena *arena, uint32_t idx)
{
	uint64_t	offset;

	offset = arena->offset - idx * arena->stride;
	return (get_ptr_from_offset(arena, offset));
}

t_token	*get_token_from_idx(t_ctx *c, uint64_t idx)
{
	t_arena	*tokens;

	tokens = &c->arena[AT_TOKENS];
	return (get_ptr_from_idx(tokens, idx));
}

char	*get_token_body(t_ctx *c, t_token *token)
{
	return (get_ptr_from_offset(&c->arena[AT_STRING], token->offset));
}

t_symbol	*get_symbol_from_top(t_ctx *c, uint32_t depth)
{
	t_arena	*stack;

	stack = &c->arena[AT_STACK];
	return (get_ptr_from_top(stack, depth));
}
