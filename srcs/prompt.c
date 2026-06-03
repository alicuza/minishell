/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 12:41:14 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/03 19:39:59 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define GREEN "\033[38;5;40m"
#define RESET "\033[0m"
#define USER "USER"
#define LOGNAME "LOGNAME"
#define ANON "anonumus"


static void add_username(t_arena	*prompt)
{
	char *user;
	user = getenv(USER);
	if(user == NULL)
		user = getenv(LOGNAME);
	if (user == NULL)
		user = ANON;
	arena_strlcat(prompt, user, ft_strlen(user) + 1);
}

char	*get_prompt(t_ctx *c)
{
	size_t	offset;
	size_t	tmp_offset;
	t_arena	*prompt;
	char *cwd;

	prompt = &(c->arena[AT_PROMPT]);
	arena_reset(prompt);
	offset = arena_strlcpy(prompt, GREEN, ft_strlen(GREEN) + 1);
	add_username(prompt);
	arena_strlcat(prompt, "@", 2);
	arena_strlcat(prompt, "shni", 5);
	arena_strlcat(prompt, ":", 2);
	tmp_offset = arena_alloc(prompt, CWD_SIZE, 1);
	cwd = getcwd(prompt->buf + tmp_offset - 1, CWD_SIZE); // TODO: is there a better way of doing this?
	if(cwd == NULL)
		printf("Arena to small"); //TODO: make proper error handling
	prompt->offset =
		prompt->sentinel + ft_strlen(prompt->buf + prompt->sentinel) + 1;
	arena_strlcat(prompt, "$ ", 3);
	arena_strlcat(prompt, RESET, ft_strlen(RESET) + 1);
	return ((char *)get_arena_ptr(prompt, offset));
}
