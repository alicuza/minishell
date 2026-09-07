#include "minishell.h"

static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}

static void	init_command(t_command_ctx *command) //, uint64_t argc)
{
	command->pathname = NULL;
	command->argc = 0;
	command->argv = NULL;
	// command->argv = malloc(sizeof(char *) * (argc + 1));
	// if (command->argv == NULL)
	// 	return (exit_mem_issue());
	// command->argv[argc] = NULL;
	//return (EXIT_SUCCESS);
}

static int set_expanded_args(t_ctx *c, t_command_ctx *command, t_list **argv)
{
	uint64_t i;
	t_list *tmp;
	
	command->argc = ft_lstsize(*argv);
	command->argv = malloc(sizeof(char *) * (command->argc + 1));
	if (command->argv == NULL)
	{
		ft_lstclear(argv, &free);
		c->should_exit = true;
		return (exit_mem_issue());
	}
	command->argv[command->argc] = NULL;
	tmp = *argv;
	i = 0;
	while (tmp != NULL)
	{
		if (command->pathname == NULL)
		{
			command->pathname = ft_strdup(tmp->content);
			if (command->pathname == NULL)
			{
				ft_lstclear(argv, &free);
				c->should_exit = true;
				return (EXIT_FAILURE);
			}
		}
		command->argv[i] = tmp->content;
		i++;
		tmp = tmp->next;
	}
	//ft_lstclear(argv, &free); TODO nik missing free
	return (EXIT_SUCCESS);
}

int	build_command(t_ctx *c, t_command_ctx *command, t_node *arg_node)
{
	t_list *argv;

	init_command(command);
	if(arg_node->type != NODE_ARG)
		return (EXIT_SUCCESS);
	argv = expand_args(c, arg_node);
	if(c->should_exit)
		return (EXIT_FAILURE);
	if(is_empty(argv->content))
	{
		ft_lstclear(&argv, &free);
		return (EXIT_SUCCESS);
	}
	return (set_expanded_args(c, command, &argv));
}
