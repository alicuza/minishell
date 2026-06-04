/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/04 17:42:07 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ctx	init_ctx(char **envp)
{
	t_ctx	c;

	ft_memset(&c, 0, sizeof(t_ctx));
	c.arena[AT_STRING] = arena_init(ARENA_SIZE, STR_SENTINEL_SIZE);
	c.arena[AT_PROMPT] = arena_init(ARENA_SIZE, STR_SENTINEL_SIZE);
	c.arena[AT_TOKEN] = arena_init(ARENA_SIZE, sizeof(t_token));
	c.env = init_env(c.env, envp);
	return (c);
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	c;
	size_t	token_idx;

	(void)argc;
	(void)argv;
#ifdef DEBUG
#endif
	c = init_ctx(envp);
	while (true)
	{
		if (!get_user_input(&c))
			break ;
#ifdef DEBUG
		//		print_arena(&c.arena[AT_PROMPT]);
		print_arena(&c.arena[AT_STRING]);
		while (true)
		{
			token_idx = get_next_token_idx(&c);
			if (!token_idx)
				break ;
			print_token(&c, token_idx);
			print_arena(&c.arena[AT_TOKEN]);
			process_token(&c, token_idx);
		}
#else
		while (true)
		{
			token_idx = get_next_token_idx(&c);
			if (!token_idx)
				break ;
			process_token(&c, token_idx);
		}
#endif
		arena_reset(&c.arena[AT_STRING]);
		arena_reset(&c.arena[AT_TOKEN]);
	}
	return (0);
}
