#include "minishell.h"

char	*expand_redir(t_ctx *c, t_node *redir_node)
{
	t_list	*list;
	char * filename;

	list = NULL;
	filename =  ft_strdup(c->arena[AT_STRING].buf + redir_node->data.redir.arena_offset);
	if (filename == NULL)
	{
		c->should_exit = true;
		return NULL;
	}
	if (redir_node->data.redir.flags & TKN_HAS_EXPANSION)
	{
		expand_word(c, &list, filename, ft_strlen(filename));
		if(ft_lstsize(list) > 1)
		{
			ft_putstr_fd(filename, STDERR_FILENO);
			ft_putstr_fd(": ambiguous redirect", STDERR_FILENO);
			c->should_exit = true;
			return NULL;
		}
		return (list->content); //TODO now we didn't clean the list 
	}
	else if (redir_node->data.redir.flags & TKN_HAS_QUOTES)
		return (quote_remove_inplace(filename));
	else
		return (filename);

}
