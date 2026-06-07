/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:45:50 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/07 16:39:36 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_user_input(t_ctx *c)
{
	size_t	offset;
	struct stat buf;

	if (fstat(STDIN_FILENO , &buf) == -1)
    return (0);
	c->read_line = readline(get_prompt(c));
	if (!c->read_line)
		return (0);
	if (*(c->read_line))
		add_history(c->read_line);
	offset =
		arena_strlcpy(&(c->arena[AT_STRING]), c->read_line, ft_strlen(c->read_line) + 1);
	// TODO: this should be implemented to concatenate the here_doc lines together until i find the `eoi/eof`
	//	arena_strlcat(&(c->arena[AT_STRING]), "\n", 2);
	free(c->read_line);
	return (offset);
}
