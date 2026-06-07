/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/07 17:08:52 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
PWD from env if he value is an absolute pathname of the current working directory that is no longer than {PATH_MAX} bytes including the terminating null byte, and the value does not contain any components that are dot or dot-dot
otherwice  pwd -P
, if there is insufficient permission on the current working directory, or on any parent of that directory, to determine what that pathname would be, the value of PWD is unspecified. Assignments to this variable may be ignored. If an application sets or unsets the value of PWD , the behaviors of the cd and pwd utilities are unspecified.
*/
static t_ctx	init_ctx(char **envp)
{
	t_ctx	c;

	ft_memset(&c, 0, sizeof(t_ctx));
	c.arena[AT_STRING] = arena_init(ARENA_SIZE, STR_SENTINEL_SIZE);
	c.arena[AT_PROMPT] = arena_init(ARENA_SIZE, STR_SENTINEL_SIZE);
	c.arena[AT_TOKEN] = arena_init(ARENA_SIZE, sizeof(t_token));
	if (init_env(&c.env, envp))
		printf("Error init_env");
	return (c);
}

int	cleanup(t_ctx	*c)
{ 
	arena_free_buf(&(c->arena[AT_STRING]));
	arena_free_buf(&(c->arena[AT_PROMPT]));
	arena_free_buf(&(c->arena[AT_TOKEN]));
	free_env(&c->env);
	return 0;
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
	cleanup(&c);
	return (c.exit_status);
}
