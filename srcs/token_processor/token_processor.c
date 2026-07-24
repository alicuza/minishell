/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/07/22 08:41:33 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

static int 	map_to_command(t_ctx *c, char *cmd)
{
	if (ft_strncmp(cmd, ENV, 4) == EQUAL)
	{
		return env(c);
	} else if (ft_strncmp(cmd, PWD, 4) == EQUAL)
	{
		return pwd(c);
	}
	else if (ft_strncmp(cmd, EXIT, 4) == EQUAL)
	{
		return builtin_exit(c);
	}
	return 0;
}

int	process_token(t_ctx *c, t_token *token)
{
	char	*content;

	if (token->type == TKN_WORD)
	{
		content = c->arena[AT_STRING].buf + token->offset;
		c->return_status = map_to_command(c, content);
	}
	return (0);
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;
	t_token	*token;
	t_arena	*stack;
	t_arena	*tokens;

	stack = &c->arena[AT_STACK];
	tokens = &c->arena[AT_TOKENS];
	while (parse->stack_idx)
	{
		symbol = get_ptr_from_idx(stack, parse->stack_idx);
		if (symbol->type == SYM_WORD)
		{
			token = get_ptr_from_idx(tokens, symbol->token_idx);
			process_token(c, token);
		}
		--parse->stack_idx;
	}
}
