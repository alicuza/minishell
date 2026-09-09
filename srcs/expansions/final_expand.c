#include "minishell.h"

//key_start is from $
//returns the len from $ to end of key body
static uint64_t	expand_var(t_ctx *c, char **new_word, char *key_start)
{
	uint64_t	end;
	char		*name;
	char		*val;

	end = get_expansion_len(key_start);
	name = ft_substr(key_start + 1, 0, end - 1);
	if (!name)
	{
		perror("expansion: ");
		cleanup(c);
		exit(EXIT_FAILURE);
	}
	val = get_expansion_value(c, name);
	free(name);
	if (!val)
		return (end);
	*new_word = ft_strjoin(*new_word, val);
	free(val);
	return (end);
}

t_list	*split_into_list(t_list **list, char *new_word, char *set)
{
	char	*split_word;
	size_t	start;
	size_t	i;

	start_or_ifs = 0;
	i = 0;
	while (true)
	{
		if (is_char_in_set(new_word[start_or_ifs + i], set))
		{
			split_word = ft_substr(new_word + start_or_ifs, 0, i);
			append_node(list, split_word);
			start_or_ifs += i + 1;
			i = 0;
		}
		else
		{
			append_node(list, new_word + start);
			break;
		}
		++i;
	}
}

void	expand_word(t_ctx *c, t_list **list, char *word, uint64_t word_len)
{
	uint64_t i;
	uint64_t start;
	char	*new_word;
	size_t	open_quote_idx;
	size_t	close_quote_idx;

	i = 0;
	start = 0;
	new_word = calloc(1, 1);
	if (get_quote_pair(word, open_quote_idx, close_quote_idx) == EXIT_FAILURE)
	{
		while (i < word_len)
		{
			if (is_expansion_start(word, i))
			{
				new_word = ft_strjoin(new_word, ft_substr(word, 0, i - 1));
				expand_var(c, new_word, word + i);
				split_into_list(t_list, new_word, IFS);
				i += get_expansion_len(word + i + 1);
			}
		}
	}
	field_split(list, new_word);
}

static void	append_expanded_nodes(t_ctx *c, t_list **list, t_node *arg_node)
{
	char	*word;

	word = ft_strdup(c->arena[AT_STRING].buf + arg_node->data.arg.arena_offset);
	if (word == NULL)
	{
		c->should_exit = true;
		ft_lstclear(list, &free);
		*list = NULL;
		return ;
	}
	if (arg_node->data.arg.flags & TKN_HAS_EXPANSION)
		expand_word(c, list, word, ft_strlen(word));
	else if (arg_node->data.arg.flags & TKN_HAS_QUOTES)
		append_node(list, quote_remove_inplace(word));
	else
		append_node(list, word);
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
