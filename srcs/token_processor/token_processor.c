/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/08/08 20:41:11 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO nik : ha? In all of the cases shown in the table where an interactive shell is required not to exit and a non-interactive shell is required to exit, an interactive shell shall not perform any further processing of the command in which the error occurred.
// TOD nik :maybe here indicate if contunue or not
void process_command(t_ctx *c, t_node			*command_node)
{
	t_node			*arg_node;
	t_command_ctx	*command;

	process_redirection(c, get_ptr_from_idx(&c->arena[AT_COMMAND], command_node->data.command.redir_head_idx));
	arg_node =  get_ptr_from_idx(&c->arena[AT_COMMAND], command_node->data.command.arg_head_idx);
	command = build_command(c, arg_node);
	command_search_and_execution(c, command); //TODO nik: make sure it folows the  Exit Status and Errors section and alos see https://www.gnu.org/software/bash/manual/bash.html#Exit-Status-1
	free(command->pathname);
	free(command->argv);
}
//TODO nik indicate if it is part of pipe or we get status right away.
void wait_return_status(t_ctx *c)
{
	// TODO nik: the exit status of a command shall be that of the last simple command executed by the command.
	- I guess trach the pid and wait for last one; and also mark shell state that it is waiting so signal hendler wkons what to do
	
	If the command is not found, the exit status shall be 127.
Otherwise, if the command name is found, but it is not an executable utility, the exit status shall be 126.
Otherwise, if the command terminated due to the receipt of a signal, the shell shall assign it an exit status greater than 128. The exit status shall identify, in an implementation-defined manner, which signal terminated the command. Note that shell implementations are permitted to assign an exit status greater than 255 if a command terminates due to a signal.
Otherwise, the exit status shall be the value obtained by the equivalent of the WEXITSTATUS macro applied to the status obtained by the wait() function (as defined in the System Interfaces volume of POSIX.1-2024). Note that for C programs, this value is equal to the result of performing a modulo 256 operation on the value passed to _Exit(), _exit(), or exit() or returned from main().

	c->return_status = 
}

// TODO nik: if it is not the only comand then it is part of pipeline and all builtins should be executed it fork. Maybe new flag is nessesary or when fork set noninteractice context: Builtin commands that are invoked as part of a pipeline, except possibly in the last element depending on the value of the lastpipe shell option (see The Shopt Builtin), are also executed in a subshell environment. Changes made to the subshell environment cannot affect the shell’s execution environment.
void process_pipeline(t_ctx *c, t_node			*pipeline_node)
{
	t_node			*command_node;
	struct stat	buf;

	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.command_head_idx);
	while(command_node != NULL)
	{
		if (command_node->data.command.next != 0)
		{
			pipe(c->pipe_fd);
			c->io_fd[1] = pipe_fd[1];
			process_command(c, command_node);
			if (fstat(STDIN_FILENO , &buf) == -1)
				return ;
			c->io_fd[0] = pipe_fd[0];
		} else
		{
			process_command(c, command_node);
			if (fstat(STDIN_FILENO , &buf) == -1)
				return ;
		}
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				command_node->data.command.next);
	}
	wait_return_status(c);
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
//TODO nik: set up https://www.gnu.org/software/bash/manual/bash.html#Signals-1
void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	(void) parse;
	// build_command(c, parse);
	execute(c); // Execute stuff after parser is done
}
