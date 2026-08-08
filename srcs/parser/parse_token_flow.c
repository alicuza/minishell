#include "minishell.h"
#include "parser.h"

bool	get_next_token(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	while (true)
	{
		if ((parse->flags & PARSE_HERE_BODY)
			&& !handle_here_body(c, parse, lex))
			return (false);
		if (parse->flags & PARSE_HAS_SAVED_TOKENS)
		{
			if (handle_saved_tokens(c, parse))
				return (true);
		}
		if (lex->flags & LEX_AT_EOI)
		{
			ft_memset(lex, 0, sizeof(t_lexer_state));
			return (false);
		}
		if (lex_token(c, lex))
		{
			++parse->token_idx;
			return (true);
		}
	}
}

void	report_parse_error(t_ctx *c, t_parser_state *parse)
{
	t_token	*token;
	char	*body;

	parse->flags |= PARSE_ERROR;
	token = get_token_from_idx(c, parse->token_idx);
	body = get_ptr_from_offset(&c->arena[AT_STRING], token->offset);
	if (parse->flags & PARSE_LOOKAHEAD_IS_EOF)
		ft_putendl_fd("minishell: syntax error near unexpected token"
			" 'end of file'", STDERR_FILENO);
	else if (body[0] == '\n')
		ft_putendl_fd("minishell: syntax error near unexpected token"
			" 'newline'", STDERR_FILENO);
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '",
			STDERR_FILENO);
		ft_putstr_fd(body, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);
	}
}

bool	get_lookahead(t_ctx *c, t_parser_state *parse, t_lexer_state *lex)
{
	while (true)
	{
		if (get_next_token(c, parse, lex))
		{
			parse->flags |= PARSE_HAS_LOOKAHEAD;
			return (true);
		}
		if (!(parse->flags & PARSE_SAVE_TOKENS))
			return (false);
		parse->flags &= ~PARSE_SAVE_TOKENS;
		parse->flags |= PARSE_HERE_BODY;
	}
}

bool	handle_here_doc(t_ctx *c, t_parser_state *parse)
{
	t_token	*cur;

	cur = get_ptr_from_idx(&c->arena[AT_TOKENS], parse->token_idx);
	if (c->arena[AT_STRING].buf[cur->offset] == '\n')
	{
		parse->flags &= ~PARSE_SAVE_TOKENS;
		parse->flags |= PARSE_HERE_BODY;
	}
	return (true);
}