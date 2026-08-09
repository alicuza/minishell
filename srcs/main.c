/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/09 11:18:17 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ctx	init_ctx(char **envp)
{
	t_ctx	c;

	ft_memset(&c, 0, sizeof(t_ctx));
	c.arena[AT_PROMPT] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_STRING] = arena_init(ARENA_SIZE, sizeof(char));
	c.arena[AT_TOKENS] = arena_init(ARENA_SIZE, sizeof(t_token));
	c.arena[AT_STACK] = arena_init(ARENA_SIZE, sizeof(t_symbol));
	c.arena[AT_COMMAND] = arena_init(ARENA_SIZE, sizeof(t_node));
	if (init_env(&c.env, envp))
		printf("Error init_env");
	if (isatty(STDIN_FILENO))
		c.is_interactive = true;
	c.io_fd[0] = -1;
	c.io_fd[1] = -1;
	c.pipe_fd[0] = -1;
	c.pipe_fd[1] = -1;
	return (c);
}

int	cleanup(t_ctx *c)
{
	arena_free(&c->arena[AT_STRING]);
	arena_free(&c->arena[AT_TOKENS]);
	arena_free(&c->arena[AT_STACK]);
	arena_free(&c->arena[AT_PROMPT]);
	arena_free(&c->arena[AT_COMMAND]);
	free_env(&c->env);
	return (0);
}

static void	shell_loop(t_ctx *c)
{
	t_parser_state	parse;

	while (true)
	{
		if (!get_user_input(c, INPUT_DEFAULT))
			break ;
		if (!*(c->read_line))
		{
			free(c->read_line);
			continue ;
		}
#ifdef DEBUG
		debug_print_read_line(c);
#endif
		parse = parse_input(c);
		if (parse.flags & PARSE_ERROR)
			c->return_status = 2;
#ifdef DEBUG
		debug_print_after_parse(c, &parse);
#endif
		free(c->read_line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	c;

	(void)argc;
	(void)argv;
	c = init_ctx(envp);
	setup_signal_handler(&c);
#ifdef DEBUG
	parse_debug_args(argc, argv, &c);
#endif
	shell_loop(&c);
	cleanup(&c);
	return (c.return_status);
}
