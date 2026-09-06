#include "minishell.h"

void	append_node(t_list **list, char *val)
{
	t_list	*new_node;

	if (val == NULL)
		return ;
	new_node = ft_lstnew(val);
	if (!new_node)
	{
		ft_lstclear(list, &free);
		*list = NULL;
		return ;
	}
	ft_lstadd_back(list, new_node);
	return ;
}

static uint64_t	expand_var(t_ctx *c, char *word, char **new_word,  uint64_t i)
{
	uint64_t	end;
	char		*name;
	char		*var_val;

	end = get_expansion_len(word + i);
	name = ft_substr(word + i + 1, 0, end - 1);
	if (!name)
	{
		perror("expansion: ");
		cleanup(c);
		exit(EXIT_FAILURE);
	}
	var_val = get_expansion_value(c, name);
	free(name);
	if (var_val)
	{
		*new_word = ft_strjoin(*new_word, var_val);
		free(var_val);
	}
	return (i + end);
}
static void expand_word(t_ctx *c, t_list **list, char *word, uint64_t len) //TODO now protect memmory
{
	uint64_t	i;
	uint64_t	start;
	char		*new_word;

	(void) list;
	i = 0;
	start = 0;
	new_word = NULL;
	while (i < len)
	{
		if (is_expansion_start(word, i))
		{
			if (new_word == NULL)
			{
				new_word = ft_substr(word, 0, i);
				// if (new_word == NULL) TODO clean list
				// 	return (EXIT_FAILURE);
			}
			else
				new_word = ft_strjoin(new_word, ft_substr(word, start, i - start));
			i = expand_var(c, word, &new_word, i);
			start = i;
		}
		else
			++i;
	}
	new_word = ft_strjoin(new_word, ft_substr(word, start, len - start + 1));
	field_split(list, new_word);
	//quote remove if (ft_strchr(val, '"') != NULL)
	//return new_word;
}

static void	append_expanded_nodes(t_ctx *c, t_list **list, t_node *arg_node)
{
	char	*val;

	val = ft_strdup(c->arena[AT_STRING].buf + arg_node->data.arg.arena_offset);
	if (val == NULL)
	{
		ft_lstclear(list, &free);
		*list = NULL;
		return ;
	}
	if (arg_node->data.arg.flags & TKN_HAS_EXPANSION)
		expand_word(c, list, val, ft_strlen(val));
	else if (arg_node->data.arg.flags & TKN_HAS_QUOTES)
		append_node(list, quote_remove_inplace(val));
	else
		append_node(list, val);
}

t_list	*expand_args(t_ctx *c, t_node *arg_node)
{
	t_list	*list;

	list = NULL;
	while (arg_node->type == NODE_ARG)
	{
		append_expanded_nodes(c, &list, arg_node);
		if (list == NULL)
			break ;
		arg_node = get_ptr_from_idx(&c->arena[AT_COMMAND], arg_node->next_idx);
	}
	return (list);
}
