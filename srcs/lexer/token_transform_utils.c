/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_transform_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:51:36 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/13 10:01:44 by sancuta          ###   ########.fr       */
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

t_symbol	*get_symbol_from_offset(t_arena *arena, uint64_t offset)
{
	return ((t_symbol *)(arena->buf + offset));
}

t_symbol	*get_symbol_from_idx(t_arena *arena, uint64_t idx)
{
	return ((t_symbol *)(arena->buf + idx * arena->stride));
}

char	*get_token_content(t_ctx *c, uint64_t offset)
{
	return (ft_strdup(c->arena[AT_STRING].buf + offset));
}
