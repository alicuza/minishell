#include "minishell.h"

void	debug_print_read_line(t_ctx *c)
{
	if (c->dbg.states & DBG_PARSER)
	{
		fprintf(stderr, "\n--- read_line ---\n");
		fprintf(stderr, "%s\n", c->read_line);
	}
	if (c->dbg.arenas & DBG_ARENA_PROMPT)
	{
		fprintf(stderr, "\n--- prompt arena after get_prompt ---\n");
		print_arena(&c->arena[AT_PROMPT]);
	}
}

void	debug_print_after_parse(t_ctx *c, t_parser_state *parse)
{
	if (c->dbg.scope & SCOPE_TOKENS)
		print_tokens(stdout, c);
	if (c->dbg.arenas & DBG_ARENA_TOKENS)
		print_arena(&c->arena[AT_TOKENS]);
	if (c->dbg.scope & SCOPE_STACK)
	{
		c->dbg.awaiting = true;
		print_stack(stdout, c, parse);
		c->dbg.awaiting = false;
	}
	if (c->dbg.arenas & DBG_ARENA_STACK)
		print_arena(&c->arena[AT_STACK]);
	if (c->dbg.scope & SCOPE_COMMAND)
		print_nodes(stdout, c);
	if (c->dbg.arenas & DBG_ARENA_COMMAND)
		print_arena(&c->arena[AT_COMMAND]);
}

void	parse_debug_args(int argc, char **argv, t_ctx *c) // TODO stefan: make these nicer
{
	static const char	*scope_names[] = {"tokens", "stack", "command",
							"trace", NULL};
	static const uint8_t	scope_bits[] = {SCOPE_TOKENS, SCOPE_STACK,
							SCOPE_COMMAND, SCOPE_TRACE};
	static const char	*state_names[] = {"lexer", "parser", "here", NULL};
	static const uint8_t	state_bits[] = {DBG_LEXER, DBG_PARSER, DBG_HEREDOC};
	static const char	*arena_names[] = {"prompt", "string", "tokens", "stack",
							"command", NULL};
	static const uint8_t	arena_bits[] = {DBG_ARENA_PROMPT, DBG_ARENA_STRING,
							DBG_ARENA_TOKENS, DBG_ARENA_STACK, DBG_ARENA_COMMAND};
	size_t	len;
	int		i;
	bool	states_seen;
	bool	arenas_seen;

	states_seen = false;
	arenas_seen = false;
	i = 1;
	while (i < argc)
	{
		len = ft_strlen(argv[i]);
		if (!ft_strncmp(argv[i], "--no_exec", len))
			c->dbg.no_exec = true;
		else if (len > 8 && (!ft_strncmp(argv[i], "--scope=", 8)
				|| !ft_strncmp(argv[i], "--tests=", 8)))
		{
			parse_flag_list(argv[i] + 8, &c->dbg.scope, scope_names, scope_bits,
				SCOPE_ALL);
			if (!c->dbg.scope)
				fprintf(stderr, "%s: matched no scope\n", argv[i]);
		}
		else if (len > 9 && !ft_strncmp(argv[i], "--states=", 9))
		{
			states_seen = true;
			parse_flag_list(argv[i] + 9, &c->dbg.states, state_names, state_bits,
				DBG_ALL_STATES);
		}
		else if (len > 9 && !ft_strncmp(argv[i], "--arenas=", 9))
		{
			arenas_seen = true;
			parse_flag_list(argv[i] + 9, &c->dbg.arenas, arena_names, arena_bits,
				DBG_ARENA_ALL);
		}
		++i;
	}
	if (!states_seen)
		c->dbg.states = DBG_ALL_STATES;
	if (!arenas_seen)
		c->dbg.arenas = 0;
}
