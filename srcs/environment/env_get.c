/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2026/09/13 20:47:33 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:47:33 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get(t_env *env, char *key)
{
	t_list	*vals;
	size_t	key_size;

	vals = env->vals;
	key_size = ft_strlen(key) + 1;
	if (vals)
	{
		while (vals)
		{
			if (!ft_strncmp(((t_env_content *)vals->content)->key, key,
					key_size))
			{
				if (((t_env_content *)vals->content)->val != NULL)
					return (ft_strdup(((t_env_content *)vals->content)->val));
				return (NULL);
			}
			vals = vals->next;
		}
	}
	return (NULL);
}
