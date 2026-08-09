/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_transform_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 16:51:36 by sancuta           #+#    #+#             */
/*   Updated: 2026/08/09 12:58:21 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_token_content(t_ctx *c, t_token *token)
{
	return (ft_strdup(c->arena[AT_STRING].buf + token->offset));
}
