/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/05 16:54:13 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_arena(t_arena *arena)
{
	char	*name;

	if (arena->stride == 1)
		name = "string (or prompt) arena";
	else if (arena->stride == sizeof(t_token))
	{
		name = "token arena";
		poison_stride(arena);
	}
	else
		name = "arena not initialized";
	fprintf(stderr, "\n%s\n\tbuffer address = %p\n\toffset = %lu\n\tcapacity = %lu\n\tstride = %lu\n", name, arena->buf, arena->offset, arena->cap, arena->stride);
	fprintf(stderr, "head:\n");
	ft_print_memory(STDERR_FILENO, arena, sizeof(t_arena));
	fprintf(stderr, "buffer:\n");
	ft_print_memory(STDERR_FILENO, arena->buf, arena->cap + arena->stride);
}

void	poison_stride(t_arena *arena)
{
	arena->buf[arena->stride - 1] = 0xFF;
}

void	print_token(t_ctx *c, size_t token_idx)
{
	char	*name;
	t_arena	*input;
	t_token	*token;

	input = &(c->arena[AT_STRING]);
	token = get_token_from_idx(&(c->arena[AT_TOKEN]), token_idx);
	if (token->token_type == TT_WORD)
		name = "WORD";
	else if (token->token_type == TT_OPERATOR)
		name = "OPERATOR";
	else
		name = "TOKEN";
	fprintf(stderr, "\ntoken\n\tstart = %lu\n\tlen = %lu\n\ttype = %u\n\tnext = %lu\n", token->content.start, token->content.len, token->token_type, token->next);
	fprintf(stderr, "%s(%.*s)\n", name, (int)token->content.len, input->buf + token->content.start);
}
