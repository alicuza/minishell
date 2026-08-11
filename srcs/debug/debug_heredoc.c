#include "minishell.h"

void	print_here_reading(t_ctx *c, t_parser_state *parse)
{
	t_arena	*strings;

	strings = &c->arena[AT_STRING];
	fprintf(stderr, "\n--- heredoc ---\n");
	fprintf(stderr, "  [reading] body until \"");
	print_escaped_str(stderr, strings->buf + parse->here.delim.pos);
	fprintf(stderr, "\"\n");
}

void	print_here_line(t_ctx *c, t_lexer_state *lex, uint64_t line, uint64_t len)
{
	fprintf(stderr, "  [line %lu] \"", line);
	print_escaped_strn(stderr, c->read_line + lex->char_idx, len);
	fprintf(stderr, "\"\n");
}

void	print_here_stored(t_parser_state *parse, uint64_t token_idx)
{
	fprintf(stderr, "  [stored] offset %lu, token_idx %lu\n",
		parse->here.body.pos, token_idx);
}

void	print_here_saving(void)
{
	fprintf(stderr, "\n--- heredoc ---\n");
	fprintf(stderr, "  [saving] entering SAVE_TOKENS mode\n");
}

void	print_here_replay(uint64_t idx)
{
	fprintf(stderr, "\n--- heredoc ---\n");
	fprintf(stderr, "  [replay] token %lu\n", idx);
}
