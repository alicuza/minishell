/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_arena.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:08:25 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 15:20:18 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	poison_stride(t_arena *arena)
{
	arena->buf[arena->stride - 1] = 0xFF;
}

const char	*get_arena_name(t_arena *arena)
{
	if (arena->stride == 1)
		return ("string/prompt arena");
	else if (arena->stride == sizeof(t_symbol))
	{
		poison_stride(arena);
		return ("stack arena");
	}
	else if (arena->stride == sizeof(t_cmd))
		return ("command arena");
	else
		return ("arena not initialized");
}

void	print_arena(t_arena *arena)
{
	fprintf(stderr, "\n--- %s ---\n", get_arena_name(arena));
	fprintf(stderr, "  {  buf = %p  offset = %lu  cap = %lu  stride = %lu  }\n",
		arena->buf, arena->offset, arena->cap, arena->stride);
	fprintf(stderr, "\n--- head ---\n");
	ft_print_memory(STDERR_FILENO, arena, sizeof(t_arena));
	fprintf(stderr, "\n--- buffer ---\n");
	ft_print_memory(STDERR_FILENO, arena->buf, arena->cap + arena->stride);
}
