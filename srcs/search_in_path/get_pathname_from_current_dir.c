/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pathname_from_current_dir.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 22:05:33 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 22:07:57 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_pathname_from_current_dir(t_command_ctx *cmd_ctx)
{
	struct stat	st;
	char		*tmp;

	tmp = ft_strjoin("./", cmd_ctx->pathname);
	if (!tmp)
		return (EXIT_FAILURE);
	if (access(tmp, F_OK) == 0 && stat(tmp, &st) == 0 && S_ISREG(st.st_mode))
	{
		free(cmd_ctx->pathname);
		cmd_ctx->pathname = tmp;
		return (EXIT_SUCCESS);
	}
	free(tmp);
	free(cmd_ctx->pathname);
	cmd_ctx->pathname = NULL;
	return (EXIT_SUCCESS);
}
