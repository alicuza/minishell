/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/27 17:51:25 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

t_command_function	match_builtin(char *cmd_ctx)
// TODO maybe use proper type defenition int (*command)(t_ctx *c);
{
	if (ft_strncmp(cmd_ctx, ENV, 4) == EQUAL)
	{
		return (&env);
	}
	else if (ft_strncmp(cmd_ctx, PWD, 4) == EQUAL)
	{
		return (&pwd);
	}
	else if (ft_strncmp(cmd_ctx, EXIT, 4) == EQUAL)
	{
		return (&builtin_exit);
	}
	else if (ft_strncmp(cmd_ctx, CD, 3) == EQUAL)
	{
		return (&builtin_exit);
	}
	return (NULL);
}

void	*search_in_path(char *cmd_ctx)
// should be actually a custom type with name params ev
{
	(void)cmd_ctx;
	return (NULL);
}

int	execute_non_builtin(t_ctx *c, char *cmd_ctx)
{
	// TODO see 2.9.1.6 Non-built-in Utility Execution https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_09_01_06
	(void)c;
	(void)cmd_ctx;
	return (0);
}

/*
	Directly follows 2.9.1.4 Command Search and Execution
	TODO nik: maybe remember its location and need not search for the utility again unless the PATH variable has been the subject of an assignment. If the remembered location fails for a subsequent invocation,
		the shell shall repeat the search to find the new location for the utility,
		if any.
*/
int	command_search_and_execution(t_ctx *c, t_command_ctx *cmd_ctx)
{
	t_command_function	command;

	command = NULL;
	if (ft_strchr(cmd_ctx->name, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->name);
		if (command != NULL)
			return (command(c, cmd_ctx));
		else
		{
			command = search_in_path(cmd_ctx->name);
			if (command != NULL)
				return (execute_non_builtin(c, cmd_ctx->name));
			else
			{
				return (127);
			}
		}
	}
	else
		return (execute_non_builtin(c, cmd_ctx->name));
}

/*
The shell parses the input into simple commands (see 2.9.1 Simple Commands) and compound commands (see 2.9.4 Compound Commands).

TODO: When Bash invokes an external command,
	the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.
*/

int	init_command(t_command_ctx *command, uint64_t argc)
{
	command->name = NULL;
	command->argc = argc;
	command->argv = malloc(sizeof(char *) * (argc + 1));
	if (command->argv == NULL)
		return (1);
	command->argv[argc] = NULL;
	return (0);
}

void	build_command(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*sym;
	uint64_t	arena_idx;
	t_arena		*stack;
	t_command_ctx	command;

	stack = &c->arena[AT_STACK];
	if (init_command(&command, parse->arena_idx))
		return ;
	arena_idx = 1;
	sym = get_symbol_from_idx(stack, arena_idx);
	while (sym->type == SYM_TOKEN && c->arena[AT_STRING].buf + sym->offset != NULL)
{
		if (command.name == NULL)
			command.name = c->arena[AT_STRING].buf + sym->offset;
		command.argv[arena_idx - 1] = c->arena[AT_STRING].buf + sym->offset;
		arena_idx++;
		sym = get_symbol_from_idx(stack, arena_idx);
	}
	c->return_status = command_search_and_execution(c, &command);
	free(command.argv);
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	build_command(c, parse);
	// t_symbol	*sym;
	// t_token		t;
	// t_arena		*stack;
	// stack = &c->arena[AT_STACK];
	// sym = get_symbol_from_idx(stack, parse->arena_idx);
	// while (1)
	// {
	// 	if (sym->type == SYM_TOKEN)
	// 	{
	// 		t.offset = sym->offset;
	// 		t.type = sym->type;
	// 		t.flags = sym->flags;
	// 		process_token(c, &t);
	// 	}
	// 	if (!sym->prev_symbol)
	// 		break ;
	// 	sym = get_symbol_from_idx(stack, sym->prev_symbol);
	// }
}
