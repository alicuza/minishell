/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:30:04 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/09 17:30:05 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static const char	*get_env_from_slice(t_env *env, const char *key,
		size_t len)
{
	t_list	*vals;

	vals = env->vals;
	while (vals)
	{
		if (ft_strncmp(((t_env_content *)vals->content)->key, key, len) == 0
			&& ((t_env_content *)vals->content)->key[len] == '\0')
		{
			if (((t_env_content *)vals->content)->val != NULL)
				return (((t_env_content *)vals->content)->val);
			return (NULL);
		}
		vals = vals->next;
	}
	return (NULL);
}

static void	arena_itoa_cat(t_ctx *c, t_expand_state *exp, int status)
{
	t_arena	*expand;
	size_t	off;

	expand = &c->arena[AT_FIELDS];
	if (!(exp->flags & EXP_HAS_FIELD))
		start_field(c, exp, "", 0);
	expand->offset -= 1;
	off = arena_itoa(expand, status);
	exp->field.len += expand->offset - off - 1;
	exp->flags |= EXP_HAS_FIELD;
}

char	*expand_var(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, char *src)
{
	uint64_t	len;
	const char	*ref;

	len = get_expansion_len(src);
	if (len == 2 && src[1] == '?')
	{
		arena_itoa_cat(c, exp, c->return_status);
		return (src + len);
	}
	ref = get_env_from_slice(&c->env, src + 1, len - 1);
	if (!ref)
		return (src + len);
	if (exp->flags & EXP_IN_DQUOTE)
		append_field(c, exp, ref, ft_strlen(ref));
	else
		append_segment(c, cmd, exp, ref);
	return (src + len);
}
