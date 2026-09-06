size_t	word_len(const char *s, char del)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != del)
		i++;
	return (i);
}

static char	**on_alloc_fail_free_str_arr(char **arr, size_t ind)
{
	while (ind-- > 0)
		free(arr[ind]);
	free(arr);
	return (NULL);
}

void	field_split(t_list **list, char *s)
{
	size_t	i;
	char	close;
	size_t	len;
	ssize_t	quote_index;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i])
	{
		quote_index = get_quote(s + i);
		if (quote_index != -1)
			split_to_last_ifs(list, s, quote_index);
		ft_lstadd_back();
		while (*s == c)
			s++;
		len = word_len(s, c);
		res[i] = ft_substr(s, 0, len);
		if (!res[i])
			return (on_alloc_fail_free_str_arr(res, i));
		s += len;
		i++;
	}
	res[word_nr] = NULL;
	return (res);
}

ssize_t	get_quote(const char *s)
{
	ssize_t	i;

	i = 0;
	while (s[i])
	{
		if (is_char_in_set(s + i, QUOTE_SET))
			return (i);
		i++;
	}
	return (-1);
}
/*
ssize_t	get_last_ifs(const char *s, ssize_t idx)
{
	while (idx > 0)
	{
		if (is_char_in_set(s + idx, IFS))
			return (idx);
		--idx;
	}
	return (-1);
}
*/

ssize_t	get_first_ifs_until(const char *s, ssize_t idx)
{
	ssize_t	i;

	i = 0;
	while (i < idx)
	{
		if (is_char_in_set(s + i, IFS))
			return (i);
		++i;
	}
	return (-1);
}

t_list	*split_to_last_ifs(t_list **list, char *s, ssize_t quote_idx)
{
	ssize_t	word_start;
	ssize_t	word_end;

	word_start = 0;
	word_end = get_first_ifs_until(s, quote_idx);
	while (word_end != -1)
	{
		append_node(list, ft_substr(s, word_start, word_end - word_start));
		word_start = word_end + 1;
		word_end = get_first_ifs_until(s + word_start, quote_idx);
	}
	if ()
}
