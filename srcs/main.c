/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/05 19:24:18 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ctx	init_ctx(char **envp)
{
	t_ctx	c;

	ft_memset(&c, 0, sizeof(t_ctx));
	c.arena[AT_STRING] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_PROMPT] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_TOKEN] = arena_init(ARENA_SIZE, sizeof(t_token));
	c.env.envp = envp;
	return (c);
}

int	main(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;

	t_ctx	c;
	size_t	token_idx;

	c = init_ctx(envp);
	while (true)
	{
		if (!get_user_input(&c, INPUT_DEFAULT))
			break ;
#ifdef DEBUG
		print_arena(&c.arena[AT_PROMPT]);
		print_arena(&c.arena[AT_STRING]);
#endif
		while (true)
		{
			token_idx = get_next_token_idx(&c);
			if (!token_idx)
				break;
			if (!ft_strncmp(c.arena[AT_STRING].buf + get_token_from_idx(&c.arena[AT_TOKEN], token_idx)->content.start, "<<", 2))
				get_user_input(&c, INPUT_CONTINUATION);
#ifdef DEBUG
			print_token(&c, token_idx);
			print_arena(&c.arena[AT_TOKEN]);
#endif
		process_token(&c, token_idx);
		}
		arena_reset(&c.arena[AT_STRING]);
		arena_reset(&c.arena[AT_TOKEN]);
	}
	return (0);
}
