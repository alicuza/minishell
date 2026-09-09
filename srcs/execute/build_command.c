#include "minishell.h"

/*static bool	is_empty(char *str)
{
	return (str == NULL || str[0] == '\0');
}
*/
void	init_command(t_ctx *c, t_command_ctx *command, t_expand_state *exp)
{
	ft_memset(command, 0, sizeof(*command));
	ft_memset(exp, 0, sizeof(*exp));
	arena_reset(&c->arena[AT_FIELDS]);
	arena_reset(&c->arena[AT_ARGV]);
}

static void	free_list_nodes(t_list **argv)
{
	t_list *tmp;
	t_list *next;

	tmp = *argv;
	while (tmp != NULL)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*argv = NULL;
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
	free_list_nodes(argv);
	return (EXIT_SUCCESS);
}

int	build_command(t_ctx *c, t_command_ctx *command, t_node *arg_node)
{
	t_list			*argv;
	t_expand_state	exp;

	init_command(c, command, &exp);
	if (arg_node->type != NODE_ARG)
		return (EXIT_SUCCESS);
	argv = expand_args(c, arg_node);
	if (c->should_exit)
	{
		ft_lstclear(&argv, &free);
		return (EXIT_FAILURE);
	}
	//if (argv == NULL)
	//	return (EXIT_SUCCESS);
	return (set_expanded_args(c, command, &argv));
}
