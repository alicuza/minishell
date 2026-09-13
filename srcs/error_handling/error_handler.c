/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/13 19:41:10 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/13 20:10:06 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_mem_error(t_ctx *c, t_command_ctx *command_ctx)
{
	t_error	e;

	e = (t_error){NULL, strerror(ENOMEM), 1};
	msh_exit(c, command_ctx, &e, NULL);
}
