/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/08/05 15:55:31 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

#define EQUAL 0

t_command_function	match_builtin(char *name)
{
	if (ft_strncmp(name, ENV, 4) == EQUAL)
		return (&env);
	else if (ft_strncmp(name, PWD, 4) == EQUAL)
		return (&pwd);
	else if (ft_strncmp(name, EXIT, 5) == EQUAL)
		return (&builtin_exit);
	else if (ft_strncmp(name, CD, 3) == EQUAL)
		return (&cd);
	else if (ft_strncmp(name, EXPORT, 7) == EQUAL)
		return (&builtin_export);
	else if (ft_strncmp(name, UNSET, 6) == EQUAL)
		return (&unset);
	else if (ft_strncmp(name, B_ECHO, 7) == EQUAL)
		return (&echo);
	else
		return (NULL);
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
	int					status;

	command = NULL;
	if (ft_strchr(cmd_ctx->pathname, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->pathname);
		if (command != NULL)
			return (command(c, cmd_ctx));
		else
		{
			status = get_pathname(c, cmd_ctx);
			if (status == EXIT_FAILURE)
				return (exit_mem_issue());
			else if (status == EXIT_SUCCESS && cmd_ctx->pathname != NULL)
				return (execute_non_builtin(c, cmd_ctx));
			else
				return (127);
		}
	}
	else
		return (execute_non_builtin(c, cmd_ctx));
}

/*
TODO nik: Maybe part of reduction:
  - read 2.6.6 Pathname Expansion
	In the case of a simple command,
		the results usually include a list of pathnames and fields to be treated as a command name and arguments; see 2.9 Shell Commands.

The shell performs redirection (see 2.7 Redirection) and removes redirection operators and their operands from the parameter list.

The shell executes a function (see 2.9.5 Function Definition Command),
	built-in (see 2.15 Special Built-In Utilities), executable file, or script,
	giving the names of the arguments as positional parameters numbered 1 to n,
	and the name of the command (or in the case of a function within a script,
	the name of the script) as special parameter 0 (see 2.9.1.4 Command Search and Execution).

The shell optionally waits for the command to complete and collects the exit status (see 2.8.2 Exit Status for Commands).
*/
void	execute(t_ctx *c, t_parser_state *parse)
{
	t_arena			*stack;
	t_symbol		*symbol;
	t_node			*pipeline_node;
	t_node			*command_node;
	t_node			*arg_node;
	t_command_ctx	*command;

	stack = &(c->arena[AT_STACK]);
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND], symbol->node_idx);
	while (*pipeline_node != '\0') // TODO stefan: check if condition is correct
	{
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.command_head_idx);
		arg_node = command_node->data.command.arg_head;
		command = build_command(c, arg_node);
		c->return_status = command_search_and_execution(c, command);
		free(command->pathname);
		free(command->argv);
		pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.next_idx);
	}
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	// build_command(c, parse);
	execute(c); // Execute stuff after parser is done
}
