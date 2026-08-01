/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/01 16:17:39 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_symbol_type	classify_token(t_ctx *c, t_token *token)
{
	char	*token_body;

	token_body = get_ptr_from_offset(&c->arena[AT_STRING], token->body.pos);
	if (!ft_strncmp(token_body, NL, ft_strlen(NL)))
		return (SYM_NEWLINE);
	else if (!ft_strncmp(token_body, DLESS, ft_strlen(DLESS) + 1))
		return (SYM_DLESS);
	else if (!ft_strncmp(token_body, DGREAT, ft_strlen(DGREAT) + 1))
		return (SYM_DGREAT);
	else if (!ft_strncmp(token_body, AND_IF, ft_strlen(AND_IF) + 1))
		return (SYM_AND_IF);
	else if (!ft_strncmp(token_body, OR_IF, ft_strlen(OR_IF) + 1))
		return (SYM_OR_IF);
	else if (!ft_strncmp(token_body, PIPE, ft_strlen(PIPE) + 1))
		return (SYM_PIPE);
	else if (!ft_strncmp(token_body, LESS, ft_strlen(LESS) + 1))
		return (SYM_LESS);
	else if (!ft_strncmp(token_body, GREAT, ft_strlen(GREAT) + 1))
		return (SYM_GREAT);
	else if (!ft_strncmp(token_body, OPAR, ft_strlen(OPAR) + 1))
		return (SYM_OPAR);
	else if (!ft_strncmp(token_body, CPAR, ft_strlen(CPAR) + 1))
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
	token = get_ptr_from_idx(tokens, parse->lookahead.token_idx);
	symbol->token_idx = parse->lookahead.token_idx;
	symbol->type = classify_token(c, token);
	symbol->entry_state = parse->cur_state;
	symbol->node_idx = 0;		// TODO: for now
}
