/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/02 20:39:29 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TOD nik :maybe here indicate if contunue or not
/*
new is pseoudo code:
	- expand args
	- expend redirects
	- if it is a pipeline then fork see is_pipe flag
	- else if builtin return

*/
void process_command(t_ctx *c, t_node			*command_node)
{
	t_node			*arg_node;
	t_command_ctx	command;

	arg_node =  get_ptr_from_idx(&c->arena[AT_COMMAND], command_node->data.command.arg_head_idx);
	if(build_command(c, &command, arg_node) == EXIT_FAILURE)
	return;
	// expand_redirections(c, get_ptr_from_idx(&c->arena[AT_COMMAND], command_node->data.command.redir_head_idx)); TODO nik
	// TODO nik: check if somthing is left after filed expansion if no just do redirection in subshell
	c->return_status = command_search_and_execution(c, &command); //TODO nik: make sure it folows the  Exit Status and Errors section and alos see https://www.gnu.org/software/bash/manual/bash.html#Exit-Status-1
	free(command.pathname);
	free(command.argv);
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

void process_pipeline(t_ctx *c, t_node			*pipeline_node)
{
	t_node			*command_node;
	int status;
	struct stat	buf;

	c->is_pipe = true;
	command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.command_head_idx);
	while(command_node->type == NODE_COMMAND)
	{
		if (command_node->next_idx != 0)
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
				command_node->next_idx);
	}
	wait_return_status(c);
	c->is_pipe = false;
}

//TODO nik: set up https://www.gnu.org/software/bash/manual/bash.html#Signals-1
void	exec_list(t_ctx *c, uint64_t head_idx)
{
	t_node		*pipeline_node;
	t_node			*command_node;

	pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND], head_idx);
	while (pipeline_node->type == NODE_PIPELINE)
	{
		command_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
				pipeline_node->data.pipeline.command_head_idx);
		if(command_node->next_idx == 0)
			process_command(c, command_node);
		else
			process_pipeline(c, pipeline_node);
		pipeline_node = get_ptr_from_idx(&c->arena[AT_COMMAND],
			pipeline_node->next_idx);
	}
}

