/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 13:03:51 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/02 13:34:01 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	start_token(t_arena *arena, size_t start, t_token_type type)
{
	t_token	*token;
	size_t	offset;

	offset = arena_alloc(arena, sizeof(t_token), _Alignof(t_token));
#ifdef DEBUG
	fprintf(stderr, "in start_token at offset %zu\n", offset);
#endif
	token = get_token_from_offset(arena, offset);
	token->content.start = start;
	token->content.len = 1;
	token->token_type = type;
	return (offset);
}

// TODO: should i reference the token itself? since now i save the corresponding arena in the token?
void	grow_token_at_idx(t_arena *arena, size_t idx)
{
	t_token	*token;

	token = get_token_from_idx(arena, idx);
	++(token->content.len);
}

void	grow_token_times_at_idx(t_arena *arena, size_t idx, size_t len)
{
	t_token	*token;

	token = get_token_from_idx(arena, idx);
	token->content.len += len;
}

void	grow_token_at_offset(t_arena *arena, size_t offset)
{
	t_token	*token;

	token = get_token_from_offset(arena, offset);
	++(token->content.len);
}

size_t	save_token_len(t_token *token)
{
	return (token->content.len);
}

void	restore_token_len(t_token *token, size_t len)
{
	token->content.len = len;
}
