/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:56:40 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:56:40 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

int	init_env(t_env *env, char **envp)
{
	char	**tmp;
	int		i;

	i = 0;
	if (envp)
	{
		while (envp[i])
		{
			tmp = ft_split_key_value(envp[i], '=');
			if (tmp == NULL)
				return (EXIT_FAILURE);
			if (is_valid_var_name(tmp[0]) == false)
			{
				free_str_arr(tmp);
				i++;
				continue ;
			}
			if (env_add(env, tmp[0], tmp[1]))
				return (free(tmp), EXIT_FAILURE);
			free(tmp);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}
