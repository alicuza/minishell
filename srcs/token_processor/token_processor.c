/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/04 18:06:22 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

void	map_to_command(t_ctx *c, char *cmd)
{
	if (ft_strncmp(cmd, ENV, 4) == EQUAL)
	{
		env(c);
	}
}

/*
TODO: When Bash invokes an external command,
	the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.
*/
int	process_token(t_ctx *c, size_t token_idx)
{
	t_token	*token;
	char	*content;

	token = get_token_from_idx(&(c->arena[AT_TOKEN]), token_idx);
	if (token->type == TT_WORD)
	{
		content = get_token_content(c, token_idx);
		map_to_command(c, content);
	}
	return (0);
}
