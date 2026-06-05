/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:28 by sancuta           #+#    #+#             */
/*   Updated: 2026/06/05 19:32:32 by sancuta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>				// printf, perror
# include <string.h>			// sterror
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
# include <limits.h>			// PATH_MAX
# include <stdint.h>			// TODO: consider for the saner type names

# include "libft.h"
# include "arena.h"

# ifdef DEBUG
#  include "debug.h"
# endif

# ifndef ARENA_SIZE
#  define ARENA_SIZE 64
# endif

# define NO_TOKEN 0

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

/* -------- quote types ----------------------------------------------------- */
// "0b" officially supported by the standard since C23, not sure if we can use it
/* TODO: probably unnecessary for me. leaving it in for now
 * # define QT_NONE 0,				// 0b00000000
 * # define QT_SQ 1,				// 0b00000001
 * # define QT_DQ 2,				// 0b00000010
 * # define QT_UNENCLOSED_SQ 8,	// 0b00000100
 * # define QT_UNENCLOSED_DQ 16,	// 0b00001000
 */

# ifndef DEBUG
typedef enum e_arena_type
{
	AT_NONE,					// TODO: should i init arenas as NONE, with everything zeroed out, and then assign arena to the correct type?
	AT_STRING,
	AT_PROMPT,
	AT_TOKEN,
	AT_STACK,
	AT_CMD,
	AT_COUNT,
}	t_arena_type;

typedef struct s_env
{
	char **envp;
}	t_env;

typedef struct s_ctx
{
	t_env	env;
	t_arena	arena[AT_COUNT];
	char  	*read_line;
	int		return_status;
}	t_ctx;
# endif

typedef struct s_slice
{
	size_t	start;
	size_t	len;
}	t_slice;

typedef enum e_token_type
{
	TT_NONE,
	TT_TOKEN,
	TT_WORD,
	TT_OPERATOR,
	TT_COUNT,
}	t_token_type;

typedef struct s_token
{
	t_slice			content;
/* TODO: probably unnecessary, because the expansion will happen directly before
 * passing to execution, so if a '$' is found in a WORD, it will be expanded
 */
	size_t			next;
	t_token_type	token_type;
/* TODO: probably unnecessary, but nice to think about, could make adding
 * features easy and it is the POSIX way to track the delimiter of a TOKEN.
 *///	char			delim_type;
}	t_token;

/* -------- main.c ---------------------------------------------------------- */
char		*get_prompt(t_ctx *c, bool with_cwd);
size_t		get_user_input(t_ctx *c, bool is_continuation);

/* -------- token.c --------------------------------------------------------- */
size_t		get_next_token_idx(t_ctx *c);

/* -------- token_transform.c ----------------------------------------------- */
size_t		get_idx_from_offset(t_arena *arena, size_t offset);
size_t		get_offset_from_idx(t_arena *arena, size_t idx);
t_token		*get_token_from_offset(t_arena *arena, size_t offset);
t_token		*get_token_from_idx(t_arena *arena, size_t idx);
char		*get_token_content(t_ctx *c, size_t token_idx);

/* -------- token_utils.c --------------------------------------------------- */
size_t		start_token(t_arena *arena, size_t start, t_token_type type);
void		grow_token_at_idx(t_arena *arena, size_t idx);
void		grow_token_times_at_idx(t_arena *arena, size_t idx, size_t len);
void		grow_token_at_offset(t_arena *arena, size_t offset);
size_t		save_token_len(t_token *arena);
void		restore_token_len(t_token *arena, size_t len);

/* -------- token_char.c ---------------------------------------------------- */
bool		is_char_in_set(char c, const char *set);
const char	**get_operator_strs(void);
bool		is_str_in_set(char *c, const char **set);

/* --------- env_utils.c ---------------------------------------------------- */
t_env		init_env(t_env env, char **envp);

/* --------- token_processor.c ---------------------------------------------- */
int			process_token(t_ctx *c, size_t token_idx);

/* --------- env.c ---------------------------------------------------------- */
void		env(t_ctx *c);

#endif
