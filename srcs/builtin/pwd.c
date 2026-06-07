/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 16:21:09 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/07 13:28:52 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

char *get_cwd_safely()
{
	char *cwd;
	cwd = getcwd(NULL,0); 
	while (!cwd)
	{
		printf("Error: failed to get cwd"); //TODO make it better
	}
	return cwd;
}

void	pwd(t_ctx *c)
{
	char* pwd;
#ifdef DEBUG
	printf("\nExecuting pwd:\n");
#endif
	pwd = search(&c->env, PWD);
	if(!pwd)
		pwd = get_cwd_safely();
	printf("%s\n", pwd);
}
