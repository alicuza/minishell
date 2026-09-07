#include "minishell.h"

size_t	get_first_ifs_until(const char *s, size_t idx)
{
	size_t	i;

	i = 0;
	while (i < idx)
	{
		if (is_char_in_set(s[i], IFS))
			return (i);
		++i;
	}
	return (i);
}

void append_splited(t_list **list, char *s, size_t	word_start, size_t	ifs_idx) 
{
	while (is_char_in_set(s[word_start], IFS) && word_start < ifs_idx)
			++word_start;
	if (word_start != ifs_idx)
		append_node(list, quote_remove_inplace(ft_substr(s, word_start, ifs_idx - word_start)));
}

void	split_to_last_ifs(t_list **list, char *s, size_t open_quote_idx, size_t close_quote_idx)
{
	size_t	word_start;
	size_t	ifs_idx;
	size_t	len;

	len = ft_strlen(s);
	if (open_quote_idx < close_quote_idx)
		len = open_quote_idx;
	word_start = 0;
	ifs_idx = get_first_ifs_until(s, len);
	while (ifs_idx < len)
	{
		append_splited(list, s, word_start, ifs_idx);
		while (is_char_in_set(*(s + ifs_idx), IFS))
			++ifs_idx;
		word_start = ifs_idx;
		ifs_idx = get_first_ifs_until(s + word_start, len);
	}
	if (ifs_idx == len)
	{
		append_splited(list, s, word_start, ifs_idx);
	}
}

int	get_quote_pair(const char *s, size_t *open, size_t *close)
{
	ssize_t	o;
	ssize_t	c;

	o = -1;
	c = -1;
	while (s[++o])
	{
		if (is_char_in_set(s[o], QUOTE_SET))
		{
			c = o;
			break;
		}
	}
	if (c == -1)
		return (EXIT_FAILURE);
	while (s[++c])
	{
		if(is_char_in_set(s[c], QUOTE_SET))
		{
			*open = o;
			*close = c;
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_FAILURE);
}

void	field_split(t_list **list, char *s)
{
	size_t	i;
	size_t	open_quote_idx;
	size_t	close_quote_idx;

	if (!s)
		return ;
	open_quote_idx = 0;
	close_quote_idx = 0;
	i = 0;
	while (s[i])
	{
		if (get_quote_pair(s + i, &open_quote_idx, &close_quote_idx) == EXIT_FAILURE)
		{
			split_to_last_ifs(list, s + i, 0, 0);
			return ;
		}
		split_to_last_ifs(list, s + i, open_quote_idx, close_quote_idx);
		i = close_quote_idx + 1;
	}
}
