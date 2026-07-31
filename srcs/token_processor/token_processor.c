/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 13:10:11 by nribakov          #+#    #+#             */
/*   Updated: 2026/07/31 12:22:03 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "env.h"
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
	int status;

	command = NULL;
	if (ft_strchr(cmd_ctx->pathname, '/') == NULL)
	{
		command = match_builtin(cmd_ctx->pathname);
		if (command != NULL)
			return (command(c, cmd_ctx));
		else
		{
			status = search_in_path(c, cmd_ctx);
			if(status == EXIT_FAILURE)
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

execve(path, array of args)


/*
The shell parses the input into simple commands (see 2.9.1 Simple Commands) and compound commands (see 2.9.4 Compound Commands).

TODO: When Bash invokes an external command,
	the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.
*/

int	init_command(t_command_ctx *command, uint64_t argc)
{
	command->pathname = NULL;
	command->argc = argc;
	command->argv = malloc(sizeof(char *) * (argc + 1));
	if (command->argv == NULL)
		return (exit_mem_issue());
	command->argv[argc] = NULL;
	return (EXIT_SUCCESS);
}

int	build_command(t_ctx *c, t_parser_state *parse)
		// TODO "export " will create env with empty key
{
	t_symbol *sym;
	uint64_t stack_idx;
	t_arena *stack;
	t_command_ctx command;

	stack = &c->arena[AT_STACK];
	if (init_command(&command, parse->stack_idx - 1))
		// TODO we have SYM_NEWLINE as last one, -1 to not cout it into argc
		return (EXIT_FAILURE);
	stack_idx = 1;
	sym = get_ptr_from_idx(stack, stack_idx);
	while (sym && sym->type == SYM_WORD) // && c->arena[AT_STRING].buf
		+ token->offset != NULL
	{
		t_token *token = get_ptr_from_idx(&c->arena[AT_TOKENS], sym->token_idx);
		if (command.name == NULL)
			command.name = c->arena[AT_STRING].buf + token->offset;
		command.argv[stack_idx - 1] = c->arena[AT_STRING].buf + token->offset;
		stack_idx++;
		sym = get_ptr_from_idx(stack, stack_idx);
	}
	c->return_status = command_search_and_execution(c, &command);
	free(command.argv);
	return (EXIT_SUCCESS);
}

void	exec_stack(t_ctx *c, t_parser_state *parse)
{
	build_command(c, parse);
	// t_symbol	*sym;
	// t_token		t;
	// t_arena		*stack;
	// stack = &c->arena[AT_STACK];
	// sym = get_ptr_from_idx(stack, parse->stack_idx);
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
	// 	sym = get_ptr_from_idx(stack, sym->prev_symbol);
	// }
}
