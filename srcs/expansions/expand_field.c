/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_field.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 17:29:22 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/09 17:29:25 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	start_field(t_ctx *c, t_expand_state *exp,
		const char *src, size_t len)
{
	t_arena	*expand;

	expand = &c->arena[AT_FIELDS];
	exp->field.pos = arena_strlcpy(expand, src, len + 1);
	exp->field.len = len;
	exp->flags |= EXP_HAS_FIELD;
}

static void	grow_field(t_ctx *c, t_expand_state *exp,
		const char *src, size_t len)
{
	t_arena	*expand;

	expand = &c->arena[AT_FIELDS];
	arena_strlcat(expand, src, len + 1);
	exp->field.len += len;
	exp->flags |= EXP_HAS_FIELD;
}

void	append_field(t_ctx *c, t_expand_state *exp,
		const char *src, size_t len)
{
	if (!(exp->flags & EXP_HAS_FIELD))
		start_field(c, exp, src, len);
	else if (len > 0)
		grow_field(c, exp, src, len);
}

void	record_reference(t_ctx *c, t_command_ctx *cmd,
		t_arena_type arena, uint32_t offset)
{
	t_arena		*arguments;
	t_argv_slot	*slot;
	size_t		slot_offset;

	arguments = &c->arena[AT_ARGV];
	slot_offset = arena_alloc(arguments, sizeof(t_argv_slot),
			_Alignof(t_argv_slot));
	slot = get_ptr_from_offset(arguments, slot_offset);
	slot->ref.arena = arena;
	slot->ref.offset = offset;
	cmd->argc += 1;
}

void	delimit_field(t_ctx *c, t_command_ctx *cmd, t_expand_state *exp)
{
	record_reference(c, cmd, AT_FIELDS, (uint32_t)exp->field.pos);
	exp->field.pos = 0;
	exp->field.len = 0;
	exp->flags &= ~EXP_HAS_FIELD;
}
