/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 08:07:58 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/28 09:37:01 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "arena.h"

typedef struct s_env_content
{
	char *key;
	char *val;
}	t_env_content;

typedef struct s_env
{
	t_list *vals;
}	t_env;

typedef enum e_arena_type
{
	AT_NONE,
	AT_PROMPT,
	AT_STRING,
	AT_STACK,
	AT_CMD,
	AT_COUNT,
}	t_arena_type;

typedef struct s_ctx
{
	t_env	env;
	t_arena	arena[AT_COUNT];
	char	*read_line;
	int		return_status;
}	t_ctx;

typedef struct s_slice
{
	uint64_t	pos;
	uint64_t	len;
}	t_slice;

typedef struct s_cmd	// stub
{
	char *path;
	char	**argv;
	char	**envp;
}	t_cmd;


typedef struct s_command_ctx
{
	char *name;
	uint64_t argc;
	char	**argv;
}	t_command_ctx;

typedef int	(*t_command_function)(t_ctx *c, t_command_ctx *command_ctx);

typedef enum e_symbol_type
{
/* -------- no token -------------------------------------------------------- */
	SYM_NONE,
/* -------- lexical tokens -------------------------------------------------- */
	SYM_TOKEN,
	SYM_OPERATOR,
	SYM_NEWLINE,
	SYM_PIPE,
	SYM_LESS,
	SYM_GREAT,
	SYM_DLESS,
	SYM_DGREAT,
	SYM_AND_IF,
	SYM_OR_IF,
	SYM_OPAR,
	SYM_CPAR,
/* -------- entrypoint ------------------------------------------------------ */
	SYM_COMPLETE_COMMAND,
/* -------- list constructs ------------------------------------------------- */
	SYM_LIST,
	SYM_AND_OR,
	SYM_TERM,
/* -------- pipeline constructs --------------------------------------------- */
	SYM_PIPELINE,
	SYM_PIPE_SEQUENCE,
/* -------- command constructs ---------------------------------------------- */
	SYM_COMMAND,
	SYM_COMPOUND_COMMAND,
	SYM_SIMPLE_COMMAND,
	SYM_COMPOUND_LIST,
	SYM_SUBSHELL,
/* -------- simple_command constructs --------------------------------------- */
	SYM_CMD_PREFIX,
	SYM_CMD_NAME,
	SYM_CMD_WORD,
	SYM_CMD_SUFFIX,
/* -------- redirection constructs ------------------------------------------ */
	SYM_IO_REDIRECT,
	SYM_IO_FILE,
	SYM_IO_HERE,
	SYM_FILENAME,
	SYM_HERE_END,
/* -------- intermediate contructs ------------------------------------------ */
	SYM_NEWLINE_LIST,
	SYM_LINEBREAK,
	SYM_SEPARATOR,
	SYM_REDIRECT_LIST,
/* -------- count ----------------------------------------------------------- */
	SYM_COUNT,
}	t_symbol_type;

typedef struct s_token
{
	uint64_t		offset;
	uint32_t		flags;
	t_symbol_type	type;
}	t_token;

typedef struct s_symbol
{
	uint64_t		offset;			/* offset into string arena */
	uint64_t		prev_symbol;	/* arena index of logical predecessor */
	uint64_t		next_frame;		/* subshell chain */
	uint64_t		next_pipeline;	/* list level pipeline chain */
	uint64_t		next_redir;		/* command level redirection chain */
	uint64_t		next_arg;		/* command level arguments chain */
	uint64_t		next_cmd;		/* pipeline level */
	uint32_t		entry_state;	/* parser state before this symbol */
	uint32_t		flags;			/* LEX_HAS_EXPANSION | LEX_HAS_QUOTES */
	t_symbol_type	type;
}	t_symbol;

typedef struct s_parser_state
{
	uint32_t	cur_state;
	uint64_t	arena_idx;
	uint64_t	stack_idx;
	t_token		lookahead;
	uint8_t		flags;				/* PARSE_HERE_PENDING | PARSE_DONE */
}	t_parser_state;

typedef struct s_lexer_state
{
	t_slice			token;			/* tracks the position and length of the current token being built */
	uint64_t		char_idx;
	t_symbol_type	type;
	uint8_t			flags;			/* LEX_HAS_EXPANSION | LEX_HAS_QUOTES | LEX_IS_BUILDING | LEX_IS_DELIMITED | LEX_NEW_INPUT */
}	t_lexer_state;

typedef struct s_split_state
{
	uint64_t	pos;
	uint64_t	len;
	bool		active;
}	t_split_state;

typedef void	(*t_reduce)(t_ctx *, t_parser_state *, uint64_t);

typedef struct s_rule
{
	t_reduce		reduce;			/* pointer to the corresponding reduce function */
	uint32_t		rhs_len;		/* number of rhs symbols in rule */
	t_symbol_type	lhs_type;
}	t_rule;

#endif
