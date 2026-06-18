/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:28 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/18 18:47:30 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>				// printf, perror
# include <string.h>			// strerror
# include <readline/readline.h>	// readline, rl_clear_history, rl_on_new_line,
								// rl_replace_line, rl_redisplay
# include <readline/history.h>	// add_history
# include <stdlib.h>			// malloc, free, exit, getenv
# include <unistd.h>			// access, write, read, fork, dup, dup2, pipe,
								// getcwd, chdir, unlink, execve,
								// isatty, ttyname, ttyslot
# include <fcntl.h>				// open, close
# include <signal.h>			// signal, sigaction, sigemptyset, sigaddset,
								// kill
# include <dirent.h>			// opendir, readdir, closedir
# include <sys/wait.h>			// wait, waitpid, wait3, wait4
# include <sys/stat.h>			// stat, lstat, fstat
# include <sys/ioctl.h>			// ioctl
# include <termios.h>			// tcgetattr, tcsetattr
# include <termcap.h>			// tgetent, tgetflag, tgetnum, tgetstr, tgoto,
								// tputs
# include <errno.h>				// errno

# include "libft.h"
# include "arena.h"
# include "types.h"

# ifdef DEBUG
#  include "debug.h"
# endif

# ifndef ARENA_SIZE
#  define ARENA_SIZE 64
# endif

# define SHELLNAME "shni"

/* -------- with_cwd -------------------------------------------------------- */
# define WITH_CWD true
# define NO_CWD false

/* -------- prompt type ----------------------------------------------------- */
# define INPUT_DEFAULT 0
# define INPUT_CONTINUATION 1

/* -------- sets ------------------------------------------------------------ */
# define OPERATOR_SET "<>&|()\n"
# define BLANK_SET " \t"
# define QUOTE_SET "\"'"
# define SPECIAL_PARAM_SET "?"

/* -------- lexer flags ----------------------------------------------------- */
# define LEX_HAS_QUOTES			0x01
# define LEX_HAS_EXPANSION		0x02
# define LEX_NEEDS_INPUT		0x04
# define LEX_IS_BUILDING		0x08
# define LEX_IS_DELIMITED		0x10

/* -------- parser flags ---------------------------------------------------- */
# define PARSE_DONE				0x01
# define PARSE_HERE_PENDING		0x02

/* -------- grammar constants ----------------------------------------------- */
# define NO_TOKEN 0
# define MAX_RHS_LEN 4
# define RULE_COUNT 48

/* -------- prompt.c --------------------------------------------------------- */
char			*get_prompt(t_ctx *c, bool with_cwd);

/* -------- input.c --------------------------------------------------------- */
char			*get_user_input(t_ctx *c, bool is_continuation);

/* -------- lookahead.c ----------------------------------------------------- */
t_token			get_lookahead(t_ctx *c, t_parser_state *p, t_lexer_state *l);

/* -------- quote_utils.c --------------------------------------------------- */
uint64_t		try_as_quote_pair(t_ctx *c, t_lexer_state *l);

/* -------- token_transform_utils.c ----------------------------------------- */
uint64_t		get_idx_from_offset(t_arena *arena, uint64_t offset);
uint64_t		get_offset_from_idx(t_arena *arena, uint64_t idx);
t_symbol		*get_symbol_from_offset(t_arena *arena, uint64_t offset);
t_symbol		*get_symbol_from_idx(t_arena *arena, uint64_t idx);
char			*get_token_content(t_ctx *c, t_symbol *symbol);

/* -------- lex_tokens.c ---------------------------------------------------- */
void			start_lex_token(t_lexer_state *lex, t_symbol_type type);
t_token			delimit_lex_token(t_ctx *, t_parser_state *, t_lexer_state *);
uint64_t		grow_lex_token(t_lexer_state *lex);

/* -------- lex_utils.c ----------------------------------------------------- */
uint64_t		consume_char(t_lexer_state *lex);
t_slice			save_lex_token_slice(t_lexer_state *lex);
void			restore_lex_token_slice(t_lexer_state *lex, t_slice len);

/* -------- string_utils.c -------------------------------------------------- */
const char		**get_operator_strs(void);
bool			is_char_in_set(char c, const char *set);
bool			is_str_in_set(char *c, const char **set);
bool			is_name_start(char c);
bool			is_name_body(char c);

/* -------- expand_utils.c -------------------------------------------------- */
bool			is_expansion_start(char *buffer, uint64_t idx);
uint64_t		get_expansion_len(char *expansion);

/* -------- env_utils.c ---------------------------------------------------- */
int	init_env(t_env *env, char **envp);
char	*search(t_env *env, char *key);

/* -------- ft_split_key_value.c ---------------------------------------------------- */
char	**ft_split_key_value(const char *s, char c);

/* -------- token_processor.c ----------------------------------------------- */
int				process_token(t_ctx *c, t_token *token);
void			exec_stack(t_ctx *c, t_parser_state *parse);

/* -------- env.c ---------------------------------------------------- */
int	env(t_ctx *c);

/* -------- pwd.c ---------------------------------------------------- */
int	pwd(t_ctx *c);
char *get_cwd_safely();

/* -------- parse_input.c --------------------------------------------------- */
t_parser_state	parse_input(t_ctx *c);

/* -------- parser_utils.c -------------------------------------------------- */
void			shift_symbol(t_ctx *c, t_parser_state *parse);

#endif

