/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/13 10:31:31 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

int 	map_to_command(t_ctx *c, char *cmd)
{
	if (ft_strncmp(cmd, ENV, 4) == EQUAL)
		return (env(c));
	else if (ft_strncmp(cmd, PWD, 4) == EQUAL)
		return (pwd(c));
	else if (ft_strncmp(cmd, EXIT, 5) == EQUAL)
		return (builtin_exit(c));
	return (0);
}

int	process_token(t_ctx *c, t_token *token)
{
	char	*content;

	if (token->type == SYM_TOKEN)
	{
		content = get_token_content(c, token->offset);
		c->exit_status = map_to_command(c, content);
		free(content);
	}
	return (0);
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*sym;
	t_token		t;
	t_arena		*stack;

	stack = &c->arena[AT_STACK];
	sym = get_symbol_from_idx(stack, parse->arena_idx);
	while (true)
	{
		if (sym->type == SYM_TOKEN)
		{
			t.offset = sym->offset;
			t.type = sym->type;
			t.flags = sym->flags;
			process_token(c, &t);
		}
		if (!sym->prev_symbol)
			break;
		sym = get_symbol_from_idx(stack, sym->prev_symbol);
	}
}
