/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:45:50 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/02 11:45:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_user_input(t_ctx *c, bool is_continuation)
{
	size_t	offset;
	char	*prompt;

	if (is_continuation)
		prompt = "> ";
	else
		prompt = get_prompt(c, true);
	c->read_line = readline(prompt);
	if (!c->read_line)
		return (0);
	if (*(c->read_line))
		add_history(c->read_line);
	offset = arena_strlcpy(&(c->arena[AT_STRING]),
			c->read_line, ft_strlen(c->read_line) + 1);
	if (is_continuation)
		arena_strlcat(&(c->arena[AT_STRING]), "\n", 2);
	free(c->read_line);
	return (offset);
}
