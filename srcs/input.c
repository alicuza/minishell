/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:45:50 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/18 19:39:47 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_user_input(t_ctx *c, bool is_continuation)
{
	char	*prompt;
	struct stat buf;

	if (fstat(STDIN_FILENO , &buf) == -1)
		return (0);
	if (is_continuation)
		prompt = "> ";
	else
		prompt = get_prompt(c, true);
	c->read_line = readline(prompt);
	if (c->read_line && *(c->read_line))
		add_history(c->read_line);
	return (c->read_line);
}
