/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_expand_no_tmp.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 10:24:34 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/09 16:31:50 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	start_field(t_ctx *c, t_expand_state *exp,
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

static void	append_field(t_ctx *c, t_expand_state *exp,
		const char *src, size_t len)
{
	if (!(exp->flags & EXP_HAS_FIELD))
		start_field(c, exp, src, len);
	else if (len > 0)
		grow_field(c, exp, src, len);
}

static void	record_reference(t_ctx *c, t_command_ctx *cmd,
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

static void	delimit_field(t_ctx *c, t_command_ctx *cmd, t_expand_state *exp)
{
	record_reference(c, cmd, AT_FIELDS, (uint32_t)exp->field.pos);
	exp->field.pos = 0;
	exp->field.len = 0;
	exp->flags &= ~EXP_HAS_FIELD;
}

static char	*open_quote(t_ctx *c, t_expand_state *exp, char *src)
{
	if (!(exp->flags & EXP_HAS_FIELD))
		start_field(c, exp, "", 0);
	if (*src == '\'')
		exp->flags |= EXP_IN_SQUOTE;
	else
		exp->flags |= EXP_IN_DQUOTE;
	return (src + 1);
}

static uint64_t	skip_ifs_run(const char *s, uint64_t i)
{
	while (s[i] && is_char_in_set(s[i], IFS))
		i++;
	return (i);
}
static uint64_t	delimit_segment(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, uint64_t i)
{
	t_arena	*expand;

	expand = &c->arena[AT_FIELDS];
	expand->buf[i] = '\0';
	if (i > exp->field.pos)
	{
		exp->field.len = i - exp->field.pos;
		delimit_field(c, cmd, exp);
	}
	else
		exp->flags &= ~EXP_HAS_FIELD;
	i = skip_ifs_run(expand->buf, i + 1);
	exp->field.pos = i;
	return (i);
}

static void	append_segment(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, const char *val)
{
	uint64_t	vlen;
	uint64_t	span;
	uint64_t	i;

	vlen = ft_strlen(val);
	if (vlen == 0)
		return ;
	append_field(c, exp, val, vlen);
	span = exp->field.pos + exp->field.len - vlen;
	i = span;
	while (i < span + vlen)
	{
		if (is_char_in_set(val[i - span], IFS))
			i = delimit_segment(c, cmd, exp, i);
		else
		{
			exp->flags |= EXP_HAS_FIELD;
			i++;
		}
	}
}

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

static char	*expand_var(t_ctx *c, t_command_ctx *cmd,
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

static bool	is_quote_border(char *word, uint64_t i)
{
	char	quote;

	quote = word[i];
	if (quote != '\'' && quote != '"')
		return (false);
	i++;
	while (word[i] && word[i] != quote)
		i++;
	return (word[i] == quote);
}

static char	*handle_squote(t_ctx *c, t_expand_state *exp, char *src)
{
	uint64_t	i;

	if (*src == '\'')
	{
		exp->flags &= ~EXP_IN_SQUOTE;
		return (src + 1);
	}
	i = 0;
	while (src[i] && src[i] != '\'')
		i++;
	append_field(c, exp, src, i);
	return (src + i);
}

static char	*handle_dquote(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, char *src)
{
	uint64_t	i;

	if (*src == '"')
	{
		exp->flags &= ~EXP_IN_DQUOTE;
		return (src + 1);
	}
	if (is_expansion_start(src, 0))
		return (expand_var(c, cmd, exp, src));
	i = 0;
	while (src[i] && src[i] != '"' && !is_expansion_start(src, i))
		i++;
	append_field(c, exp, src, i);
	return (src + i);
}

static char	*handle_unquoted(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, char *src)
{
	uint64_t	i;

	if (is_quote_border(src, 0))
		return (open_quote(c, exp, src));
	if (is_expansion_start(src, 0))
		return (expand_var(c, cmd, exp, src));
	i = 0;
	while (src[i] && !is_quote_border(src, i)
		&& !is_expansion_start(src, i))
		i++;
	append_field(c, exp, src, i);
	return (src + i);
}

static void	scan_word(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, char *word)
{
	char	*src;

	src = word;
	while (*src)
	{
		if (exp->flags & EXP_IN_SQUOTE)
			src = handle_squote(c, exp, src);
		else if (exp->flags & EXP_IN_DQUOTE)
			src = handle_dquote(c, cmd, exp, src);
		else
			src = handle_unquoted(c, cmd, exp, src);
	}
	if (exp->flags & EXP_HAS_FIELD)
		delimit_field(c, cmd, exp);
}

void	finish_args(t_ctx *c, t_command_ctx *command)
{
	t_arena		*arguments;
	t_argv_slot	*slot;
	uint64_t	i;
	uint64_t	first;

	arguments = &c->arena[AT_ARGV];
	slot = get_ptr_from_offset(arguments, arena_alloc(arguments,
				sizeof(t_argv_slot), _Alignof(t_argv_slot)));
	slot->ptr = NULL;
	first = get_idx_from_offset(arguments, arguments->stride);
	i = 0;
	while (i < command->argc)
	{
		slot = get_ptr_from_idx(arguments, first + i);
		slot->ptr = (char *)get_ptr_from_offset(
				&c->arena[slot->ref.arena], slot->ref.offset);
		i++;
	}
	command->argv = (char **)get_ptr_from_idx(arguments, first);
}

void	expand_args_arena(t_ctx *c, t_command_ctx *command,
		t_expand_state *exp, t_node *arg_node)
{
	t_node	*node;
	char	*word;

	node = arg_node;
	while (node->type == NODE_ARG)
	{
		word = get_ptr_from_offset(&c->arena[AT_STRING],
			node->data.arg.arena_offset);
		if (!(node->data.arg.flags & (TKN_HAS_QUOTES | TKN_HAS_EXPANSION)))
			record_reference(c, command, AT_STRING,
				(uint32_t)node->data.arg.arena_offset);
		else
			scan_word(c, command, exp, word);
		node = get_ptr_from_idx(&c->arena[AT_COMMAND], node->next_idx);
	}
}

static void	resolve_redir_field(t_ctx *c, t_node *redir_node, size_t redir_base)
{
	t_arena			*arguments;
	t_argv_slot	*slot;

	arguments = &c->arena[AT_ARGV];
	slot = get_ptr_from_offset(&c->arena[AT_ARGV], redir_base);
	redir_node->data.redir.arena_offset = slot->ref.offset;
	arena_restore(arguments, redir_base);
}

int	expand_redir_arena(t_ctx *c, t_node *redir_node, t_expand_state *exp)
{
	t_arena			*arguments;
	t_arena			*strings;
	t_command_ctx	tmp;
	size_t			redir_base;
	char			*word;

	strings = &c->arena[AT_STRING];
	arguments = &c->arena[AT_ARGV];
	if (redir_node->flags & REDIR_HERE)
		return (EXIT_SUCCESS);
	ft_memset(&tmp, 0, sizeof(tmp));
	ft_memset(exp, 0, sizeof(*exp));
	word = get_ptr_from_offset(strings, redir_node->data.redir.arena_offset);
	if (!(redir_node->data.redir.flags & (TKN_HAS_QUOTES | TKN_HAS_EXPANSION)))
		return (EXIT_SUCCESS);
	redir_base = arena_save(arguments);
	scan_word(c, &tmp, exp, word);
	if (tmp.argc != 1)
	{
		arena_restore(arguments, redir_base);
		return (EXIT_FAILURE);
	}
	resolve_redir_field(c, redir_node, redir_base);
	return (EXIT_SUCCESS);
}
