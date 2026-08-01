/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_transform_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:51:36 by sancuta           #+#    #+#             */
/*   Updated: 2026/07/22 09:43:06 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint64_t	get_idx_from_offset(t_arena *arena, uint64_t offset)
{
	return (offset / arena->stride);
}

uint64_t	get_offset_from_idx(t_arena *arena, uint64_t idx)
{
	return (idx * arena->stride);
}

void	*get_ptr_from_offset(t_arena *arena, uint64_t offset)
{
	return ((arena->buf + offset));
}

void	*get_ptr_from_idx(t_arena *arena, uint64_t idx)
{
	return ((arena->buf + idx * arena->stride));
}

char	*get_token_content(t_ctx *c, t_token *token)
{
	return (ft_strdup(c->arena[AT_STRING].buf + token->body.pos));
}
