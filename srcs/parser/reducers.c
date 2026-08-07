# define OFFSET 21

static t_rule	*rule_dispatch_first(int32_t action); //TODO stefan: is index same as bison or it is -1
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
