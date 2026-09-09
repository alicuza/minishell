/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sancuta <sancuta@student.42vienna.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 20:02:32 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/09 09:03:37 by sancuta          ###   ########.fr       */
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

static uint64_t	get_seg_len(const char *s, uint64_t i)
{
	uint64_t	len;

	len = 0;
	while (s[i + len] && !is_char_in_set(s[i + len], IFS))
		len++;
	return (len);
}

static void	append_segment(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, const char *val)
{
	uint64_t	start;
	uint64_t	len;

	start = 0;
	while (val[start])
	{
		if (is_char_in_set(val[start], IFS))
		{
			if (exp->flags & EXP_HAS_FIELD)
				delimit_field(c, cmd, exp);
			start = skip_ifs_run(val, start);
		}
		else
		{
			len = get_seg_len(val, start);
			append_field(c, exp, val + start, len);
			start += len;
		}
	}
}

static char	*get_env_from_slice(t_env *env, const char *key, size_t len,
		t_arena *scratch)
{
	t_list	*vals;

	vals = env->vals;
	while (vals)
	{
		if (ft_strncmp(((t_env_content *)vals->content)->key, key, len) == 0
			&& ((t_env_content *)vals->content)->key[len] == '\0')
		{
			if (((t_env_content *)vals->content)->val != NULL)
				return (get_ptr_from_offset(scratch, arena_strlcpy(scratch,
						((t_env_content *)vals->content)->val,
						ft_strlen(((t_env_content *)vals->content)->val)
						+ 1)));
			return (NULL);
		}
		vals = vals->next;
	}
	return (NULL);
}

static char	*fetch_expansion_value(t_ctx *c, char *src, uint64_t len,
		t_arena *scratch)
{
	if (len == 2 && src[1] == '?')
		return (get_ptr_from_offset(scratch,
				arena_itoa(scratch, c->return_status)));
	return (get_env_from_slice(&c->env, src + 1, len - 1, scratch));
}

static char	*expand_var(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, char *src)
{
	uint64_t	len;
	t_arena		*scratch;
	size_t		save;
	char		*val;

	len = get_expansion_len(src);
	scratch = &c->arena[AT_SCRATCH];
	save = arena_save(scratch);
	val = fetch_expansion_value(c, src, len, scratch);
	if (!val)
	{
		arena_restore(scratch, save);
		return (src + len);
	}
	if (exp->flags & EXP_IN_DQUOTE)
		append_field(c, exp, val, ft_strlen(val));
	else
		append_segment(c, cmd, exp, val);
	arena_restore(scratch, save);
	return (src + len);
}

static bool	is_quote_border(char *word, uint64_t i)
{
	char	q;

	q = word[i];
	if (q != '\'' && q != '"')
		return (false);
	i++;
	while (word[i] && word[i] != q)
		i++;
	return (word[i] == q);
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

static void	expand_word_dispatch(t_ctx *c, t_command_ctx *cmd,
		t_expand_state *exp, t_node *arg_node)
{
	char	*word;

	word = c->arena[AT_STRING].buf + arg_node->data.arg.arena_offset;
	if (!(arg_node->data.arg.flags & (TKN_HAS_QUOTES | TKN_HAS_EXPANSION)))
		record_reference(c, cmd, AT_STRING,
			(uint32_t)arg_node->data.arg.arena_offset);
	else
		scan_word(c, cmd, exp, word);
}
