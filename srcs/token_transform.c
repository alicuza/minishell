/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_transform.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:51:36 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/03 21:40:32 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	get_idx_from_offset(t_arena *arena, size_t offset)
{
	return (offset / arena->stride);
}

size_t	get_offset_from_idx(t_arena *arena, size_t idx)
{
	return (idx * arena->stride);
}

t_token	*get_token_from_offset(t_arena *arena, size_t offset)
{
	return ((t_token *)(arena->buf + offset));
}

t_token	*get_token_from_idx(t_arena *arena, size_t idx)
{
	return ((t_token *)(arena->buf + idx * arena->stride));
}

char *get_token_content(t_ctx *c, size_t token_idx)
{
	t_arena	*input;
	t_token	*token;
	char	*content;

	input = &(c->arena[AT_STRING]);
	token = get_token_from_idx(&(c->arena[AT_TOKEN]), token_idx);
	content = malloc(token->content.len + 1);
	if (!content)
		return (NULL);
	ft_strlcpy(content, input->buf + token->content.start, token->content.len + 1);
	return (content);
}
