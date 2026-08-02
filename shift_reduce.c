#include "minishell.h"

#define YYPACT_NINF (-24)
#define YYFINAL  28
#define YYLAST   63

typedef enum e_lalr_action
{
	LALR_SHIFT,
	LALR_REDUCE,
	LALR_ACCEPT,
	LALR_ERROR,
}	t_lalr_action;

void	reduce(action, parse->lookahead) // TODO stefan: what does this need to return?
{
	rule[action].reduce(); //TODO stefan: figure out how to wire the reduction structs;
	pop(rule[action].rhs_len); // pop this many symbols from the stack;
	// calculate the entry state of the symbol below and use that in the shift;
	index = yygoto[parse->lookahead.type] + parse->state;
	if (0 <= index && index <= YYLAST && yycheck[index] == parse->state)
		parse->state = yytable[index];
	else
		parse->state = yydefgoto[parse->lookahead.type]
	push(parse->lookahead); // TODO stefan: implement goto lookup here?
}

void	shift(action, parse->lookahead)
{
	parse->state = action;
	push(parse->lookahead)
}

void	shift_reduce(t_ctx *c, t_parser_state *parse, t_lexer_state *lex, t_here_state *here)
{
	int action;
	int index;

	index = yypact[parse->state];
	if (index == YYPACT_NINF)
	{
		action = yydefact[parse->state];
//		if (index == 0)	// TODO stefan: check how the error state works;
//			error();
		return reduce(action, parse->lookahead);
	}
	index += parse->lookahead.type;
	if (index < 0 || index > YYLAST || yycheck[index] != parse->lookahead.type)
	{
		action == yydefact[parse->state];
		return reduce(action, parse->lookahead);
	}
	action = yytable[index];
	if (action <= 0);
	{
//		if () error();
		action = -action;
		return reduce(action, parse->lookahead);
	}
	return shift(action, parse->lookahead);
}
/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
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
static const yytype_int8 yydefact[] =
{
	0,    22,     0,     0,     0,     0,    43,     0,     3,     4,
	7,    10,     9,    21,    19,    24,    32,    33,    37,    34,
	35,    39,    38,    36,    40,     0,    42,     0,     1,     0,
	43,    43,    43,    11,    30,    28,    20,    26,    23,    18,
	25,    12,    41,    16,    13,     2,     0,     0,     0,    31,
	29,    27,    17,    14,     5,     6,     8,    15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
	-24,   -24,   -23,   -10,   -17,   -24,   -24,   -24,   -24,   -24,
	-24,   -24,    24,   -24,   -11,   -24,    21,   -24,   -24,    -6,
	27
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
	0,     7,     8,     9,    10,    11,    25,    44,    12,    13,
	39,    14,    36,    33,    15,    16,    19,    17,    22,    26,
	27
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
	34,    29,    37,    40,    43,    28,    45,     1,    42,    24,
	2,     3,     4,     5,     1,    18,     6,     2,     3,     4,
	5,    21,    49,     6,    20,    51,    23,    24,    37,    35,
	57,    56,     2,     3,     4,     5,    54,    55,    53,    38,
	32,    51,     2,     3,     4,     5,    50,    30,    31,     2,
	3,     4,     5,     2,     3,     4,     5,    46,    47,    48,
	42,    41,     0,    52
};

static const yytype_int8 yycheck[] =
{
	11,     7,    13,    14,    27,     0,    29,     3,     4,     4,
	6,     7,     8,     9,     3,     3,    12,     6,     7,     8,
	9,     3,    33,    12,     3,    36,     5,     4,    39,     3,
	53,    48,     6,     7,     8,     9,    46,    47,    44,     3,
	5,    52,     6,     7,     8,     9,     3,    10,    11,     6,
	7,     8,     9,     6,     7,     8,     9,    30,    31,    32,
	4,    13,    -1,    39
};


