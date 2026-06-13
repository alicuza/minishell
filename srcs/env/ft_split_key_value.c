/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_key_value.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 09:26:43 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/13 09:42:22 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "types.h"

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

char	**ft_split_key_value(const char *s, char c)
{
	char	**res;
	size_t	len;

	if (!s)
		return (NULL);
	res = malloc((3) * sizeof (char *));
	if (!res)
		return (NULL);
  len = word_len(s, c);
  res[0] = ft_substr(s, 0, len);
  if (!res[0])
    return (on_alloc_fail_free_str_arr(res, 0));
  s+= len + 1;
  res[1] = ft_substr(s, 0, ft_strlen(s));
  if (!res[1])
    return (on_alloc_fail_free_str_arr(res, 1));
	res[2] = NULL;
	return (res);
}
