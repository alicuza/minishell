/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:53:16 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/04 16:18:46 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_ctx *c)
{
  #ifdef DEBUG
  printf("\nExecuting env:\n");
  #endif
  int i = 0;
  char **envp = c->env.envp;
  if(envp)
    while(envp[i] != NULL)
    {
      printf("%s\n", envp[i]);
      i++;
    }
}
