/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/18 21:16:18 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

/*
	Since cd is only implemented 'Intrinsic Utility' it is also mapped here
*/
void 	 *match_builtin(char *cmd) //TODO maybe use proper type defenition int (*command)(t_ctx *c);
{
	if (ft_strncmp(cmd, ENV, 4) == EQUAL)
	{
		return (&env);
	} else if (ft_strncmp(cmd, PWD, 4) == EQUAL)
	{
		return (&pwd);
	}
	else if (ft_strncmp(cmd, EXIT, 4) == EQUAL)
	{
		return (&builtin_exit);
	}
	return NULL;
}

void *search_in_path(char *cmd) // should be actually a custom type with name params ev 
{
	(void) cmd;
	return NULL;
}

int execute_non_builtin(t_ctx *c, char *cmd)
{
	// TODO see 2.9.1.6 Non-built-in Utility Execution https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_09_01_06
	(void) c;
	(void) cmd;
	return 0;
}

/*
	Directly follows 2.9.1.4 Command Search and Execution
	TODO nik: maybe remember its location and need not search for the utility again unless the PATH variable has been the subject of an assignment. If the remembered location fails for a subsequent invocation, the shell shall repeat the search to find the new location for the utility, if any.
*/
int command_search_and_execution(t_ctx *c, char *cmd)
{
	int (*command)(t_ctx *c);

	command = NULL;
	if (ft_strchr(cmd, '/') == NULL)
	{
		command = match_builtin(cmd);
		if (command != NULL)
			return command(c);
		else
		{
			command = search_in_path(cmd);
			if (command != NULL)
				return execute_non_builtin(c, cmd); 
			else
			{
				//TODO Write error: Command 'lol' not found (, did you mean: , but can be installed with:) should we add this posfix?
				return 127;
			}
		}
	}
	else
		return execute_non_builtin(c, cmd);
}

/*
The shell parses the input into simple commands (see 2.9.1 Simple Commands) and compound commands (see 2.9.4 Compound Commands).

TODO: When Bash invokes an external command,
	the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.
*/

int	process_token(t_ctx *c, t_token *token) // see 2.9.1.1 Order of Processing
{
	char	*content;

	/*
	1.IS simpele comand? A "simple command" is a sequence of optional variable assignments and redirections, in any sequence, optionally followed by words and redirections.
	1.1 variable assignments or redirections are saved for alter processing.	
	*/

	if (token->type == SYM_TOKEN)
	{
		content = c->arena[AT_STRING].buf + token->offset;
		c->return_status = command_search_and_execution(c, content);
	}
	return (0);
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*sym;
	t_token		t;
	t_arena		*stack;

	stack = &c->arena[AT_STACK];
	sym = get_symbol_from_idx(stack, parse->arena_idx);
	while (1)
	{
		if (sym->type == SYM_TOKEN)
		{
			t.offset = sym->offset;
			t.type = sym->type;
			t.flags = sym->flags;
			process_token(c, &t);
		}
		if (!sym->prev_symbol)
			break;
		sym = get_symbol_from_idx(stack, sym->prev_symbol);
	}
}
