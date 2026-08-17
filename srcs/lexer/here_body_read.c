#include "minishell.h"

static void	warn_here_eof(t_ctx *c, t_here_state *here)
{
	char	*delim;

	delim = get_ptr_from_offset(&c->arena[AT_STRING], here->delim.pos);
	ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
	ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
	ft_putstr_fd(delim, STDERR_FILENO);
	ft_putendl_fd("')", STDERR_FILENO);
}

static bool	read_here_line(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	get_user_input(c, INPUT_CONTINUATION);
	if (!c->read_line)
	{
		warn_here_eof(c, here);
		memset(&here->body, 0, sizeof(t_slice));
		delimit_lex_here(c, here);
		return (false);
	}
	ft_memset(lex, 0, sizeof(t_lexer_state));
	return (true);
}

static void	grow_here_body(t_ctx *c, t_lexer_state *lex, t_here_state *here,
		uint64_t len)
{
	t_arena	*strings;

	strings = &c->arena[AT_STRING];
	if (!here->body.len)
		here->body.pos = arena_strlcpy(strings, c->read_line + lex->char_idx,
				len + 2);
	else
		arena_strlcat(strings, c->read_line + lex->char_idx, len + 2);
	here->body.len += len + 1;
	consume_char(lex, len + 1);
}

bool	is_delim_line(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	t_arena		*strings;
	char		*line;

	strings = &c->arena[AT_STRING];
	line = c->read_line + lex->char_idx;
	return (!ft_strncmp(line, strings->buf + here->delim.pos, here->delim.len)
		&& (line[here->delim.len] == '\n'));
}

void	get_here_doc(t_ctx *c, t_lexer_state *lex, t_here_state *here)
{
	uint64_t	len;
#ifdef DEBUG
	uint64_t	line;

	line = 0;
#endif
	while (true)
	{
		if (!c->read_line[lex->char_idx]
			&& !read_here_line(c, lex, here))
			return;
		len = word_len(c->read_line + lex->char_idx, '\n');
		if (is_delim_line(c, lex, here))
		{
			consume_char(lex, len + 1);
			delimit_lex_here(c, here);
			return;
		}
#ifdef DEBUG
		if (c->dbg.states & DBG_HEREDOC)
			print_here_line(c, lex, ++line, len);
#endif
		grow_here_body(c, lex, here, len);
	}
}