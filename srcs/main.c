/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:47:55 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/08 13:26:04 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
PWD from env if the value is an absolute pathname of the current working
directory that is no longer than {PATH_MAX} bytes including the terminating
null byte, and the value does not contain dot or dot-dot components
otherwice  pwd -P
if the cwd or any parent directory have insufficient permission, to determine
what that pathname would be, the value of PWD is unspecified. Assignments to
this variable may be ignored. If an application sets or unsets PWD, the
behaviors of the cd and pwd utilities are unspecified.
*/
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
#ifdef DEBUG
	parse_debug_args(argc, argv, &c);
#endif
	shell_loop(&c);
	cleanup(&c);
	return (c.return_status);
}
