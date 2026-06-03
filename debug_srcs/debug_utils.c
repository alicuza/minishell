/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:29 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/03 21:40:34 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	print_arena(t_arena *arena)
{
	char	*name;

	if (arena->sentinel == 1)
		name = "string (or prompt) arena";
	else if (arena->sentinel == sizeof(t_token))
	{
		name = "token arena";
		poison_sentinel(arena);
	}
	else
		name = "arena not initialized";
	printf("\n%s\n\tbuffer address = %p\n\toffset = %lu\n\tcapacity = %lu\n\tsentinel = %lu\n", name, arena->buf, arena->offset, arena->cap, arena->sentinel);
	printf("head:\n");
	ft_print_memory(arena, sizeof(t_arena));
	printf("buffer:\n");
	ft_print_memory(arena->buf, arena->cap + arena->sentinel);
}

void	poison_sentinel(t_arena *arena)
{
	arena->buf[arena->sentinel - 1] = 0xFF;
}

void	print_token(t_ctx *c, size_t token_idx)
{
	char	*name;
	t_arena	*input;
	t_token	*token;

	input = &(c->arena[AT_STRING]);
	token = get_token_from_idx(&(c->arena[AT_TOKEN]), token_idx);
	if (token->type == TT_WORD)
		name = "WORD";
	else if (token->type == TT_OPERATOR)
		name = "OPERATOR";
	else
		name = "TOKEN";
	printf("\ntoken\n\tstart = %lu\n\tlen = %lu\n\ttype = %u\n\tnext = %lu\n", token->content.start, token->content.len, token->type, token->next);
	printf("%s(\"%.*s\")\n", name, (int)token->content.len, input->buf + token->content.start);
}
