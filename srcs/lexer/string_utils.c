/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 17:03:42 by sancuta           #+#    #+#             */
/*   Updated: 2026/05/26 19:14:10 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_char_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (true);
		++set;
	}
	return (false);
}

const char	**get_operator_strs(void)
{
	static const char	*set[] = {"<<", ">>", "&&", "||", NULL};

	return (set);
}

bool	is_str_in_set(char *c, const char **set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (!ft_strncmp(c, set[i], 2))
			return (true);
		++i;
	}
	return (false);
}

bool	is_name_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}

bool	is_name_body(char c)
{
	return (ft_isalnum((unsigned char)c) || c == '_');
}
