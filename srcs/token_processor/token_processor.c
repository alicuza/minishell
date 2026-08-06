/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/08/07 00:19:54 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

void process_command(t_ctx *c, t_node			*command_node)
{
	t_node			*arg_node;
	t_command_ctx	*command;

	arg_node =  get_ptr_from_idx(&c->arena[AT_COMMAND], command_node->data.command.arg_head_idx);
	command = build_command(c, arg_node);
	c->return_status = command_search_and_execution(c, command);
	free(command->pathname);
	free(command->argv);
}

void process_pipeline(t_ctx *c, t_node			*pipeline_node)
{
	t_node			*command_node;

	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.command_head_idx);
	while(command_node != NULL)
	{
		process_command(c, command_node);
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				command_node->data.command.next);
	}
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
void	execute(t_ctx *c)
{
	t_arena			*stack;
	t_symbol		*symbol;
	t_node			*pipeline_node;

	stack = &(c->arena[AT_STACK]);
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND], symbol->node_idx);
	while (pipeline_node != NULL) // TODO stefan: check if condition is correct
	{
		process_pipeline(c, pipeline_node);
		pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.next_idx);
	}
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	(void) parse;
	// build_command(c, parse);
	execute(c); // Execute stuff after parser is done
}
