#include "minishell.h"

#define YYPACT_NINF (-24)
#define YYFINAL  28
#define YYLAST   63
#define NTERM_OFFSET 14

//TODO stefan: wrap these into functions
/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const int32_t yypact[] =
{
	11,   -24,    12,    12,    18,    12,    23,     5,    37,    35,
	-24,    47,   -24,    26,    36,   -24,   -24,   -24,   -24,   -24,
	-24,   -24,   -24,   -24,   -24,    48,    56,    11,   -24,     4,
	23,    23,    23,    47,   -24,   -24,    43,   -24,   -24,    26,
	-24,   -24,   -24,    37,    23,    37,    11,    11,    11,   -24,
	-24,   -24,    43,     4,    35,    35,   -24,    37
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const int32_t yydefact[] =
{
	0,    22,     0,     0,     0,     0,    43,     0,     3,     4,
	7,    10,     9,    21,    19,    24,    32,    33,    37,    34,
	35,    39,    38,    36,    40,     0,    42,     0,     1,     0,
	43,    43,    43,    11,    30,    28,    20,    26,    23,    18,
	25,    12,    41,    16,    13,     2,     0,     0,     0,    31,
	29,    27,    17,    14,     5,     6,     8,    15
};

/* YYPGOTO[NTERM-NUM].  */
static const int32_t yypgoto[] =
{
	-24,   -24,   -23,   -10,   -17,   -24,   -24,   -24,   -24,   -24,
	-24,   -24,    24,   -24,   -11,   -24,    21,   -24,   -24,    -6,
	27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const int32_t yydefgoto[] =
{
	0,     7,     8,     9,    10,    11,    25,    44,    12,    13,
	39,    14,    36,    33,    15,    16,    19,    17,    22,    26,
	27
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const int32_t yytable[] =
{
	34,    29,    37,    40,    43,    28,    45,     1,    42,    24,
	2,     3,     4,     5,     1,    18,     6,     2,     3,     4,
	5,    21,    49,     6,    20,    51,    23,    24,    37,    35,
	57,    56,     2,     3,     4,     5,    54,    55,    53,    38,
	32,    51,     2,     3,     4,     5,    50,    30,    31,     2,
	3,     4,     5,     2,     3,     4,     5,    46,    47,    48,
	42,    41,     0,    52
};

static const int32_t yycheck[] =
{
	11,     7,    13,    14,    27,     0,    29,     3,     4,     4,
	6,     7,     8,     9,     3,     3,    12,     6,     7,     8,
	9,     3,    33,    12,     3,    36,     5,     4,    39,     3,
	53,    48,     6,     7,     8,     9,    46,    47,    44,     3,
	5,    52,     6,     7,     8,     9,     3,    10,    11,     6,
	7,     8,     9,     6,     7,     8,     9,    30,    31,    32,
	4,    13,    -1,    39
};

typedef enum e_lalr_action
{
	LALR_SHIFT,
	LALR_REDUCE,
	LALR_ACCEPT,
	LALR_ERROR,
}	t_lalr_action;

static void pop(t_ctx *c, t_parser_state *parse, uint32_t len)
{
	t_arena		*stack;
	t_symbol	*symbol;

	stack = &(c->arena[AT_STACK]);
	parse->stack_idx -= len;
	stack->offset -= len * stack->stride;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	parse->state = symbol->entry_state;
}

void	push_term(t_ctx *c, t_parser_state *parse)
{
	t_symbol	*symbol;
	t_arena		*stack;
	t_arena		*tokens;
	t_token		*token;

	stack = &(c->arena[AT_STACK]);
	tokens = &(c->arena[AT_TOKENS]);
	parse->stack_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	token = get_ptr_from_idx(tokens, parse->token_idx);
	symbol->token_idx = parse->token_idx;
	symbol->type = classify_token(c, token);
	symbol->entry_state = parse->state;
	symbol->node_idx = 0;		// TODO: for now
}

void	push_nonterm(t_ctx *c, t_parser_state *parse, t_symbol_type type)
{
	t_symbol	*symbol;
	t_arena		*stack;

	stack = &(c->arena[AT_STACK]);
	parse->stack_idx = get_idx_from_offset(stack,
			arena_alloc(stack, sizeof(t_symbol), _Alignof(t_symbol)));
	symbol = get_ptr_from_idx(stack, parse->stack_idx);
	symbol->token_idx = parse->token_idx;
	symbol->type = type;
	symbol->entry_state = parse->state;
	symbol->node_idx = 0;		// TODO: for now
}

//typedef struct s_rule		// TODO stefan: delete this when done with rules
//{
//	t_reduce		handler;		/* pointer to handler function */
//	uint32_t		rhs_len;		/* number of rhs symbols in rule */
//	t_symbol_type	lhs_type;		/* type of lhs in rule */
//}	t_rule;


# define OFFSET 21

static t_rule	*rule_dispatch_first(int32_t action) //TODO stefan: is index same as bison or it is -1
{
	t_rule	rule[20];
	rule[23 - OFFSET] = {NULL, 1, SYM_CMD_PREFIX};
	rule[24 - OFFSET] = {NULL, 2, SYM_CMD_PREFIX};
	rule[25 - OFFSET] = {NULL, 1, SYM_CMD_SUFFIX};
	rule[26 - OFFSET] = {NULL, 2, SYM_CMD_SUFFIX};
	rule[27 - OFFSET] = {NULL, 1, SYM_CMD_SUFFIX};
	rule[28 - OFFSET] = {NULL, 2, SYM_CMD_SUFFIX};
	rule[29 - OFFSET] = {NULL, 1, SYM_REDIRECT_LIST};
	rule[30 - OFFSET] = {NULL, 2, SYM_REDIRECT_LIST};
	rule[31 - OFFSET] = {NULL, 1, SYM_IO_REDIRECT};
	rule[32 - OFFSET] = {NULL, 1, SYM_IO_REDIRECT};
	rule[33 - OFFSET] = {&reduce_io_file_LESS, 2, SYM_IO_FILE};
	rule[34 - OFFSET] = {&reduce_io_file_GREAT, 2, SYM_IO_FILE};
	rule[35 - OFFSET] = {&reduce_io_file_DGREAT, 2, SYM_IO_FILE};
	rule[36 - OFFSET] = {&reduce_filename, 1, SYM_FILENAME};
	rule[37 - OFFSET] = {&reduce_io_here, 2, SYM_IO_HERE};
	rule[38 - OFFSET] = {&reduce_here_end, 1, SYM_HERE_END};
	rule[39 - OFFSET] = {NULL, 1, SYM_SEPARATOR};
	rule[40 - OFFSET] = {NULL, 2, SYM_SEPARATOR};
	rule[41 - OFFSET] = {NULL, 1, SYM_LINEBREAK};
	rule[42 - OFFSET] = {NULL, 1, SYM_LINEBREAK};
	return (rule[action]);
}

static t_rule	*rule_dispatch_second(int32_t action);
{
	t_rule	rule[22];
	rule[0] = memset(rule, 0, sizeof(t_rule));
	rule[1] = {NULL, 3, SYM_COMPLETE_COMMANDS};
	rule[2] = {NULL, 1, SYM_COMPLETE_COMMANDS};
	rule[3] = {NULL, 1, SYM_LIST};
	rule[4] = {NULL, 4, SYM_LIST};
	rule[5] = {NULL, 4, SYM_LIST};
	rule[6] = {NULL, 1, SYM_PIPELINE};
	rule[7] = {NULL, 4, SYM_PIPELINE};
	rule[8] = {NULL, 1, SYM_COMMAND};
	rule[9] = {NULL, 1, SYM_COMMAND};
	rule[10] = {NULL, 2, SYM_COMMAND};
	rule[11] = {NULL, 3, SYM_SUBSHELL};
	rule[12] = {NULL, 2, SYM_COMPOUND_LIST};
	rule[13] = {NULL, 3, SYM_COMPOUND_LIST};
	rule[14] = {NULL, 3, SYM_TERM};
	rule[15] = {NULL, 1, SYM_TERM};
	rule[16] = {NULL, 3, SYM_SIMPLE_COMMAND};
	rule[17] = {NULL, 2, SYM_SIMPLE_COMMAND};
	rule[18] = {NULL, 1, SYM_SIMPLE_COMMAND};
	rule[19] = {NULL, 2, SYM_SIMPLE_COMMAND};
	rule[20] = {NULL, 1, SYM_SIMPLE_COMMAND};
	rule[21] = {&reduce_cmd_name, 1, SYM_CMD_NAME};
	rule[22] = {&reduce_cmd_word, 1, SYM_CMD_WORD};
	return (rule[action]);
}

static t_rule	*get_rule(uint32_t action)
{
	if (0 < action && action < 23)
		return rule_dispatch_first(action);
	else
		return rule_dispatch_second(action);
}

void	reduce(t_ctx *c, t_parser_state *parse, int32_t action)// TODO stefan: what does this need to return?
{
	t_rule	*rule;
	int32_t	lhs;
	int32_t	index;

	rule = get_rule(action);
	lhs = rule->lhs_type - NTERM_OFFSET;
	rule->handler(c, parse); //TODO stefan: figure out how to wire the reduction structs;
	pop(c, parse, rule->rhs_len); // pop this many symbols from the stack;
	index = yypgoto[lhs] + parse->state; // calculate the entry state of the symbol below and use that in the shift;
	if (0 <= index && index <= YYLAST && yycheck[index] == parse->state)
		parse->state = yytable[index];
	else
		parse->state = yydefgoto[lhs];
	push_nonterm(c, parse, rule->lhs_type);
}

void	shift(t_ctx *c, t_parser_state *parse, int32_t action)
{
	parse->state = action;
	push_term(c, parse);
}

void	shift_reduce(t_ctx *c, t_parser_state *parse, t_lexer_state *lex, t_here_state *here)
{
	t_arena	*tokens;
	int32_t	action;
	int32_t	index;
	int32_t	lookahead_type;

	(void) lex; //TODO stefan: check if nessesary
	(void) here; //TODO stefan: use it for here io
	tokens = &(c->arena[AT_TOKENS]);
	index = yypact[parse->state];
	if (index == YYPACT_NINF)
	{
		action = yydefact[parse->state];
//		if (index == 0)	// TODO stefan: check how the error state works;
//			error();
		return reduce(c, parse, action);
	}

	lookahead_type = classify_token(c, get_ptr_from_idx(tokens, parse->token_idx));

	index += lookahead_type;
	if (index < 0 || index > YYLAST || yycheck[index] != lookahead_type)
	{
		action = yydefact[parse->state];
		return reduce(c, parse, action);
	}
	action = yytable[index]; //TODO stefan: we don't have negative values do we need this part?
	if (action <= 0)
	{
//		if () error();
		action = -action;
		return reduce(c, parse, action);
	}
	return shift(c, parse, action);
}

void	reduce_cmd_name(t_ctx *c, t_parser_state *parse)
{
	uint64_t	cmd_offset;
	t_arena		*tokens;
	t_arena		*stack;
	t_arena		*commands;
	t_token		*token;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	tokens = &c->arena[AT_TOKENS];
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	cmd_offset = arena_alloc(commands, sizeof(t_node), _Alignof(t_node));
	node = get_ptr_from_offset(commands, cmd_offset);
	node->type = NODE_ARG;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	symbol->node_idx = get_idx_from_offset(commands, cmd_offset);
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	node->data.arg.arena_offset = token->offset;
}

void	reduce_cmd_word(t_ctx *c, t_parser_state *parse)
{
	uint64_t	cmd_offset;
	t_arena		*tokens;
	t_arena		*stack;
	t_arena		*commands;
	t_token		*token;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	tokens = &c->arena[AT_TOKENS];
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	cmd_offset = arena_alloc(commands, sizeof(t_node), _Alignof(t_node));
	node = get_ptr_from_offset(commands, cmd_offset);
	node->type = NODE_ARG;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	symbol->node_idx = get_idx_from_offset(commands, cmd_offset);
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	node->data.arg.arena_offset = token->offset;
}

void	reduce_here_end(t_ctx *c, t_parser_state *parse)
{
	uint64_t	cmd_offset;
	t_arena		*tokens;
	t_arena		*stack;
	t_arena		*commands;
	t_token		*token;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	tokens = &c->arena[AT_TOKENS];
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	cmd_offset = arena_alloc(commands, sizeof(t_node), _Alignof(t_node));
	node = get_ptr_from_offset(commands, cmd_offset);
	node->type = NODE_REDIR;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	symbol->node_idx = get_idx_from_offset(commands, cmd_offset);
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	node->data.arg.arena_offset = token->offset;
}

void	reduce_filename(t_ctx *c, t_parser_state *parse)
{
	uint64_t	cmd_offset;
	t_arena		*tokens;
	t_arena		*stack;
	t_arena		*commands;
	t_token		*token;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	tokens = &c->arena[AT_TOKENS];
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	cmd_offset = arena_alloc(commands, sizeof(t_node), _Alignof(t_node));
	node = get_ptr_from_offset(commands, cmd_offset);
	node->type = NODE_REDIR;
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	symbol->node_idx = get_idx_from_offset(commands, cmd_offset);
	token = get_ptr_from_idx(tokens, symbol->token_idx);
	node->data.arg.arena_offset = token->offset;
}

void	reduce_io_here(t_ctx *c, t_parser_state *parse) // TODO stefan: should this be the function that triggers the here_body reading?
{
	(void) c; //TODO stefan: check if nessesary
	(void) parse; //TODO stefan: check if nessesary
}

void	reduce_io_file_LESS(t_ctx *c, t_parser_state *parse)
{
	t_arena		*stack;
	t_arena		*commands;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	node = get_ptr_from_idx(commands, symbol->node_idx);
	node->flags |= REDIR_IN;
}

void	reduce_io_file_GREAT(t_ctx *c, t_parser_state *parse)
{
	t_arena		*stack;
	t_arena		*commands;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	node = get_ptr_from_idx(commands, symbol->node_idx);
	node->flags |= REDIR_OUT;
}

void	reduce_io_file_DGREAT(t_ctx *c, t_parser_state *parse)
{
	t_arena		*stack;
	t_arena		*commands;
	t_symbol	*symbol;
	t_node		*node;

	(void) parse; //TODO stefan: check if nessesary
	commands = &c->arena[AT_COMMAND];
	stack = &c->arena[AT_STACK];
	symbol = get_ptr_from_offset(stack, stack->offset - stack->stride);
	node = get_ptr_from_idx(commands, symbol->node_idx);
	node->flags |= REDIR_APPEND;
}
