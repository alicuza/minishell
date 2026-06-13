/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/12 17:19:38 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ctx	init_ctx(char **envp)
{
	t_ctx	c;

	ft_memset(&c, 0, sizeof(t_ctx));
	c.arena[AT_STRING] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_PROMPT] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_STACK] = arena_init(ARENA_SIZE, sizeof(t_symbol));
//	c.arena[AT_CMD] = arena_init(ARENA_SIZE, sizeof(t_cmd));
	c.env = init_env(c.env, envp);
	return (c);
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	c;

	(void) argc;
	(void) argv;
	c = init_ctx(envp);
	while (true)
	{
		if (!get_user_input(&c, INPUT_DEFAULT))
			break ;
		if (!*(c.read_line))
		{
			free(c.read_line);
			continue ;
		}
#ifdef DEBUG
		fprintf(stderr, "\n--- read_line ---\n");
		fprintf(stderr, "%s\n", c.read_line);
		fprintf(stderr, "\n--- prompt arena after get_prompt ---\n");
		print_arena(&c.arena[AT_PROMPT]);
#endif
		t_parser_state	parse;

		parse = parse_input(&c);
#ifdef DEBUG
		print_complete_stack(&c, &parse);
#endif
		exec_stack(&c, &parse);
		arena_reset(&c.arena[AT_STRING]);
		arena_reset(&c.arena[AT_STACK]);
		free(c.read_line);
	}
	arena_free(&c.arena[AT_STRING]);
	arena_free(&c.arena[AT_PROMPT]);
	arena_free(&c.arena[AT_STACK]);
//	arena_free(&c.arena[AT_CMD]);
	return (0);
}
