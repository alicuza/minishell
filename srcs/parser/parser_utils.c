/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/26 18:48:32 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol_type	classify_token(t_ctx *c, t_token *token)
{
	char	*token_body;

	token_body = get_ptr_from_offset(&c->arena[AT_STRING], token->offset);
	if (!ft_strncmp(token_body, NL, ft_strlen(NL)))
		return (SYM_NEWLINE);
	else if (!ft_strncmp(token_body, DLESS, ft_strlen(DLESS)))
		return (SYM_DLESS);
	else if (!ft_strncmp(token_body, DGREAT, ft_strlen(DGREAT)))
		return (SYM_DGREAT);
	else if (!ft_strncmp(token_body, AND_IF, ft_strlen(AND_IF)))
		return (SYM_AND_IF);
	else if (!ft_strncmp(token_body, OR_IF, ft_strlen(OR_IF)))
		return (SYM_OR_IF);
	else if (!ft_strncmp(token_body, PIPE, ft_strlen(PIPE)))
		return (SYM_PIPE);
	else if (!ft_strncmp(token_body, LESS, ft_strlen(LESS)))
		return (SYM_LESS);
	else if (!ft_strncmp(token_body, GREAT, ft_strlen(GREAT)))
		return (SYM_GREAT);
	else if (!ft_strncmp(token_body, OPAR, ft_strlen(OPAR)))
		return (SYM_OPAR);
	else if (!ft_strncmp(token_body, CPAR, ft_strlen(CPAR)))
		return (SYM_CPAR);
	return (SYM_WORD);
}

void	shift_symbol(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;
	t_arena	*stack;
	t_arena	*tokens;
	t_token	*token;

	stack = &(c->arena[AT_STACK]);
	tokens = &(c->arena[AT_TOKENS]);
	parse->stack_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	token = get_ptr_from_idx(tokens, parse->token_idx);
	symbol->token_idx = parse->token_idx;
	symbol->type = classify_token(c, token);
	symbol->entry_state = parse->cur_state;
	symbol->node_idx = 0;		// TODO: for now
}
