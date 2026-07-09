/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:45:50 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/27 23:12:40 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_user_input(t_ctx *c, bool is_continuation)
{
	char	*prompt;
	struct stat buf;

	if (fstat(STDIN_FILENO , &buf) == -1)
		return (0);
	if (!c->is_interactive)
	{
		prompt = NULL;
		rl_outstream = stderr;			// redirects the readline output to stderr - so it doesn't mirror the input.
	}
	else if (is_continuation)
		prompt = "> ";
	else
		prompt = get_prompt(c, true);
	c->read_line = readline(prompt);
	if (c->read_line && *(c->read_line) && c->is_interactive)
		add_history(c->read_line);
	return (c->read_line);
}
