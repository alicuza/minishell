#include "minishell.h"

static int	init_command(t_command_ctx *command, uint64_t argc)
{
	command = malloc(sizeof(t_command_ctx));
	if(command == NULL)
		return (EXIT_FAILURE);
	command->pathname = NULL;
	command->argc = argc;
	command->argv = malloc(sizeof(char *) * (argc + 1));
	if (command->argv == NULL)
		return (exit_mem_issue());
	command->argv[argc] = NULL;
	return (EXIT_SUCCESS);
}

int	build_command_from_token(t_ctx *c, t_parser_state *parse)
{
	t_symbol		*sym;
	uint64_t		stack_idx;
	t_arena			*stack;
	t_command_ctx	command;
	t_token			*token;

	stack = &c->arena[AT_STACK];
	if (init_command(&command, parse->stack_idx - 1))
		return (EXIT_FAILURE);
	stack_idx = 1;
	sym = get_ptr_from_idx(stack, stack_idx);
	while (sym && sym->type == SYM_WORD)
	{
		token = get_ptr_from_idx(&c->arena[AT_TOKENS], sym->token_idx);
		if (command.pathname == NULL)
		{
			command.pathname = ft_strdup(c->arena[AT_STRING].buf
					+ token->offset);
			if (command.pathname == NULL)
				return (EXIT_FAILURE);
		}
		command.argv[stack_idx - 1] = c->arena[AT_STRING].buf + token->offset;
		stack_idx++;
		sym = get_ptr_from_idx(stack, stack_idx);
	}
	c->return_status = command_search_and_execution(c, &command);
	free(command.pathname);
	free(command.argv);
	return (EXIT_SUCCESS);
}

static int	get_argc(t_ctx *c, t_node *arg_node)
{
	int	i;

	i = 0;
	while (arg_node != NULL)
	{
		arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND], arg_node->next_idx);
		i++;
	}
	return (i);
}
//TODO nik: do all expansions
//  - read 2.6.6 Pathname Expansion
t_command_ctx	*build_command(t_ctx *c, t_node *arg_node)
{
	t_command_ctx	*command;
	int				i;

	i = 0;
	command = NULL;
	if (init_command(command, get_argc(c, arg_node)))
		return (NULL);
	while (arg_node != NULL)
	{
		if (command->pathname == NULL) // TODO nik: also meanse that it is first arg The first word (if any) that is not a variable assignment or redirection shall be expanded. If any fields remain following its expansion, the first field shall be considered the command name.  If no fields remain, the next word (if any) shall be expanded, and so on, until a command name is found or no words remain. see 3.5 Shell Expansions in bash man
		{
			command->pathname = ft_strdup(c->arena[AT_STRING].buf
					+ arg_node->data.arg.arena_offset);
			if (command->pathname == NULL)
				return (NULL);
		}
		command->argv[i] = c->arena[AT_STRING].buf + arg_node->data.arg.arena_offset;
		i++;
		arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND], arg_node->next_idx);
	}
	return (command);
}
