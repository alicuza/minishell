/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 20:43:39 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:43:40 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "minishell.h"

#define EQUAL 0

int	unset(t_ctx *c, t_command_ctx *command_ctx)
{
	uint64_t	i;

	i = 1;
	while (i < command_ctx->argc)
	{
		if (ft_strncmp(command_ctx->argv[i], SHLVL, 6) != EQUAL)
			env_delete(&c->env, command_ctx->argv[i]);
		i++;
	}
	return (0);
}
