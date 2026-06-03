/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:56:10 by nribakov          #+#    #+#             */
/*   Updated: 2026/06/01 18:56:10 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define ENV "env"
# define EQUAL 0

/*
TODO: When Bash invokes an external command, the variable ‘$_’ is set to the full pathname of the command and passed to that command in its environment.
*/
int	process_token(t_ctx *c, size_t token_idx)
{
  t_token	*token;
  char *content;

  token = get_token_from_idx(&(c->arena[AT_TOKEN]), token_idx);
  if (token->type == TT_WORD)
  {
    content = get_token_content(c, token_idx);
    printf("\nWill execute builin\n");
    if(ft_strncmp(content, ENV, 4) == EQUAL)
     env(c);
  }
  
  return 0;
}