/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/08/09 13:36:06 by nribakov         ###   ########.fr       */
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
/*
TODO nik:  the exit status of a command shall be that of the last simple command executed by the command.
	- Maybe indicate if it is part of pipe or we get status right away.
		+ store list of child processes, and if is not empaty wait here
	- I guess trach the pid and wait for last one; and also mark shell state that it is waiting so signal hendler wkons what to do

*/
void wait_return_status(t_ctx *c)
{
	(void) c;

// 	If the command is not found, the exit status shall be 127.
// Otherwise, if the command name is found, but it is not an executable utility, the exit status shall be 126.
// Otherwise, if the command terminated due to the receipt of a signal, the shell shall assign it an exit status greater than 128. The exit status shall identify, in an implementation-defined manner, which signal terminated the command. Note that shell implementations are permitted to assign an exit status greater than 255 if a command terminates due to a signal.
// Otherwise, the exit status shall be the value obtained by the equivalent of the WEXITSTATUS macro applied to the status obtained by the wait() function (as defined in the System Interfaces volume of POSIX.1-2024). Note that for C programs, this value is equal to the result of performing a modulo 256 operation on the value passed to _Exit(), _exit(), or exit() or returned from main().
// c->return_status = 
}

// TODO nik: if it is not the only comand then it is part of pipeline and all builtins should be executed it fork. Maybe new flag is nessesary or when fork set noninteractice context: Builtin commands that are invoked as part of a pipeline, except possibly in the last element depending on the value of the lastpipe shell option (see The Shopt Builtin), are also executed in a subshell environment. Changes made to the subshell environment cannot affect the shell’s execution environment.
void process_pipeline(t_ctx *c, t_node			*pipeline_node)
{
	t_node			*command_node;
	int status;
	struct stat	buf;


	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.command_head_idx);
	while(command_node != NULL)
	{
		if (command_node->data.command.next != 0)
		{
			status = pipe(c->pipe_fd);
			if(status == -1)
				return ; // TODO:nik make proper error handling
			c->io_fd[1] = c->pipe_fd[1];
			process_command(c, command_node);
			if (fstat(STDIN_FILENO , &buf) == -1)
				return ; //TODO nik: currently will miss exit status, we might need to wait for started chiled
			c->io_fd[0] = c->pipe_fd[0];
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

//TODO nik: set up https://www.gnu.org/software/bash/manual/bash.html#Signals-1
void	exec_list(t_ctx *c, uint64_t head_idx)
{
	t_node		*pipeline_node;

	pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND], head_idx);
	while (pipeline_node != NULL)
	{
		process_pipeline(c, pipeline_node);
		if(pipeline_node->next_idx)
			pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->next_idx);
		else
			return;
	}
}

