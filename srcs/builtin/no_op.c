/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   no_op.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 00:53:05 by nribakov          #+#    #+#             */
/*   Updated: 2026/09/14 00:56:30 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	no_op(t_ctx *c, t_command_ctx *command_ctx)
{
	(void)c;
	(void)command_ctx;
	return (EXIT_SUCCESS);
}
