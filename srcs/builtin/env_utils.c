/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:54:27 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/01 18:54:27 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
//TODO nik: set mininal required enviroment var
default ones
PWD=/home/nikit/42/minishell
LS_COLORS=
LESSCLOSE=/usr/bin/lesspipe %s %s
LESSOPEN=| /usr/bin/lesspipe %s
SHLVL=2
PATH=/home/nikit/.local/funcheck/host:/home/nikit/.local/funcheck/host:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
_=/usr/bin/env*/
t_env	init_env(t_env env, char **envp)
{
  env.envp = envp;
  return env;
}
