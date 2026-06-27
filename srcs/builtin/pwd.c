/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 16:21:09 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/18 20:46:54 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env.h"

char *get_pwd()
{
	char *pwd;
	pwd = search(&c->env, PWD);
	if(!pwd)
		pwd = getcwd(NULL,0);
#ifdef DEBUG
	if (!pwd)
	{
		fprintf(stderr, "Error: failed to get cwd");
	}
#endif
	return pwd;
}

int	pwd(t_ctx *c, char **argv)
{
	char* pwd;
#ifdef DEBUG
	printf("\nExecuting pwd:\n");
#endif
	(void) argv;
	pwd = get_pwd();
	if(!pwd)
		return 1;
	printf("%s\n", pwd);
	return 0;
}
