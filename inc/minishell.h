/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nribakov <nribakov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/22 21:48:28 by sancuta           #+#    #+#             */
/*   Updated: 2026/09/06 00:12:50 by nribakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>             // printf, perror
# include <string.h>            // strerror
# include <readline/readline.h> // readline, rl_clear_history, rl_on_new_line,
								// rl_replace_line, rl_redisplay
# include <readline/history.h>  // add_history
# include <stdlib.h>            // malloc, free, exit, getenv
# include <unistd.h>            // access, write, read, fork, dup, dup2, pipe,
								// getcwd, chdir, unlink, execve,
								// isatty, ttyname, ttyslot
# include <fcntl.h>             // open, close
# include <signal.h>            // signal, sigaction, sigemptyset, sigaddset,
								// kill
# include <dirent.h>            // opendir, readdir, closedir
# include <sys/ioctl.h>         // ioctl
# include <sys/stat.h>          // stat, lstat, fstat
# include <sys/wait.h>          // wait, waitpid, wait3, wait4
# include <termcap.h>           // tgetent, tgetflag, tgetnum, tgetstr, tgoto,
# include <termios.h>           // tcgetattr, tcsetattr
								// tputs
# include "arena.h"
# include "libft.h"
# include "parser.h"
# include "types.h"
# include <errno.h> // errno

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

/* -------- heredoc --------------------------------------------------------- */
# define HEREDOC_TMP "/tmp/.msh_heredoc_"

/* -------- operators ------------------------------------------------------- */
# define NL "\n"
# define PIPE "|"
# define LESS "<"
# define GREAT ">"
# define DLESS "<<"
# define DGREAT ">>"
# define AND_IF "&&"
# define OR_IF "||"
# define OPAR "("
# define CPAR ")"

/* -------- sets ------------------------------------------------------------ */
# define OPERATOR_SET "<>&|()\n"
# define BLANK_SET " \t"
# define QUOTE_SET "\"'"
# define SPECIAL_PARAM_SET "?"

/* -------- lexer flags ----------------------------------------------------- */
# define TKN_HAS_QUOTES			0x01
# define TKN_HAS_EXPANSION		0x02
# define LEX_IS_BUILDING		0x04
# define LEX_AT_EOI				0x08
# define LEX_INTERRUPTED		0x10

/* -------- parser flags ---------------------------------------------------- */
# define PARSE_SAVE_TOKENS		0x01
# define PARSE_HERE_BODY		0x02
# define PARSE_HAS_SAVED_TOKENS	0x04
# define PARSE_HAS_LOOKAHEAD	0x08
# define PARSE_DONE				0x10
# define PARSE_ERROR			0x20
# define PARSE_INTERRUPTED		0x40

/* -------- node flags ------------------------------------------------------ */
# define FLAG_AND_IF 0x01
# define FLAG_OR_IF 0x02
# define FLAG_SUBSHELL 0x04
# define REDIR_IN 0x08
# define REDIR_OUT 0x10
# define REDIR_HERE 0x20
# define REDIR_APPEND 0x40
# define REDIR_HAS_QUOTES 0x80

# ifdef DEBUG
/* -------- test scope flags ------------------------------------------------ */
#  define SCOPE_TOKENS 0x01
#  define SCOPE_STACK 0x02
#  define SCOPE_COMMAND 0x04
#  define SCOPE_TRACE 0x08
#  define SCOPE_ALL 0x0f

/* -------- debug state flags (--states=) ----------------------------------- */
#  define DBG_LEXER 0x01
#  define DBG_PARSER 0x02
#  define DBG_HEREDOC 0x04
#  define DBG_ALL_STATES 0x07

/* -------- debug arena flags (--arenas=) ----------------------------------- */
#  define DBG_ARENA_PROMPT 0x01
#  define DBG_ARENA_STRING 0x02
#  define DBG_ARENA_TOKENS 0x04
#  define DBG_ARENA_STACK 0x08
#  define DBG_ARENA_COMMAND 0x10
#  define DBG_ARENA_ALL 0x1f

/* -------- parser output sub-toggles (--parser=) --------------------------- */
/* which parts of the DBG_PARSER trace to print, per shift/reduce step */
#  define DBG_SHOW_FLAGS 0x01  /* banner + parse flags line */
#  define DBG_SHOW_STACK 0x02  /* vertical symbol stack */
#  define DBG_SHOW_ACTION 0x04 /* the shift/reduce action line */
#  define DBG_SHOW_NODES 0x08  /* node arena dump after a reduce */
#  define DBG_SHOW_LINKS 0x10  /* [rhs]/[lhs] node context lines */
#  define DBG_SHOW_ALL 0x1f

/* -------- lookahead sentinel labels --------------------------------------- */
#  define DBG_LOOKAHEAD_PENDING "(pending)"
#  define DBG_LOOKAHEAD_EOF "SYM_EOF"

/* -------- default debug config (edit these to change what shows) ---------- */
/* applied when the matching --states=/--parser=/--scope=/--arenas= is absent */
#  define DBG_DEFAULT_STATES DBG_ALL_STATES
#  define DBG_DEFAULT_PARSER DBG_SHOW_ALL
#  define DBG_DEFAULT_SCOPE 0
#  define DBG_DEFAULT_ARENAS 0
# endif

/* -------- grammar constants ----------------------------------------------- */
# define NO_TOKEN 0
# define MAX_RHS_LEN 4
# define RULE_COUNT 46

/* -------- globals.c ------------------------------------------------------- */
extern volatile sig_atomic_t	g_signal;

/* -------- cleanup.c ---------------------------------------------------------- */
int				cleanup(t_ctx *c);
void			close_io(t_ctx *c);
void			free_str_arr(char **val);

/* -------- prompt.c -------------------------------------------------------- */
char			*get_prompt(t_ctx *c, bool with_cwd);

/* -------- input.c --------------------------------------------------------- */
char			*get_user_input(t_ctx *c, bool is_continuation);

/* -------- lookahead.c ----------------------------------------------------- */
bool			lex_token(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_1(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_2(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_3(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_4(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_5(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_6(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_7(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_8(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_9(t_ctx *c, t_lexer_state *lex);
bool			apply_rule_10(t_ctx *c, t_lexer_state *lex);

/* -------- pair_utils.c ---------------------------------------------------- */
char			matching_close(char open);
bool			find_matched_pair(t_ctx *c, t_lexer_state *lex, char open);

/* -------- token_transform_utils.c ----------------------------------------- */
char			*get_token_content(t_ctx *c, t_token *token);

/* -------- lex_tokens.c ---------------------------------------------------- */
void			start_lex_token(t_lexer_state *lex, t_token_type type);
uint64_t		alloc_token(t_ctx *c);
void			delimit_lex_token(t_ctx *c, t_lexer_state *lex);
uint64_t		grow_lex_token(t_lexer_state *lex, uint64_t len);

/* -------- lex_heredoc.c --------------------------------------------------- */
void			handle_here_body(t_ctx *c, t_parser_state *p, t_lexer_state *l);
void			handle_saved_tokens(t_ctx *c, t_parser_state *parse);
void			handle_pipe_error(t_ctx *c);

/* -------- here_body_read.c ------------------------------------------------ */
void			get_here_doc(t_ctx *c, t_lexer_state *l);

/* -------- here_read_line.c ------------------------------------------------ */
bool			read_here_line(t_ctx *c, t_lexer_state *l, char *here_end);
bool			here_line_ends(t_ctx *c, t_lexer_state *l, char *here_end);

/* -------- here_write_line.c ----------------------------------------------- */
void			write_here_line(t_ctx *c, int fd, t_lexer_state *l,
					t_node *node);

/* -------- lex_utils.c ----------------------------------------------------- */
uint64_t		consume_char(t_lexer_state *lex, uint64_t len);
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

/* -------- env_add.c ------------------------------------------------------- */
int				env_add(t_env *env, char *key, char *value);

/* -------- env_update.c ---------------------------------------------------- */
int				env_update(t_env *env, char *key, char *value);
int				env_update_with_copy(t_env *env, char *key, char *value);

/* -------- env_get.c ------------------------------------------------------- */
char			*env_get(t_env *env, char *key);

/* -------- env_delete.c ---------------------------------------------------- */
void			env_delete(t_env *env, char *key);

/* -------- free_env.c ------------------------------------------------------ */
void			free_env_content(void *content_void_p);
void			free_env(t_env *env);

/* -------- init_env.c ------------------------------------------------------ */
int				init_env(t_env *env, char **envp);

/* -------- add_env_defaults.c ---------------------------------------------- */
int				add_env_defaults(t_env *env);

/* -------- env_to_envp.c --------------------------------------------------- */
char			**env_to_envp(t_env *env);

/* -------- ft_split_key_value.c -------------------------------------------- */
char			**ft_split_key_value(const char *s, char c);

/* -------- execute_list.c -------------------------------------------------- */
void			execute_list(t_ctx *c, uint64_t head_idx);

/* -------- execute_pipeline.c ---------------------------------------------- */
void			execute_pipeline(t_ctx *c, t_node *pipeline_node);

/* -------- execute_simple_command.c ---------------------------------------- */
void			execute_simple_command(t_ctx *c, t_node *command_node);

/* -------- build_command.c ------------------------------------------------- */
int				build_command(t_ctx *c, t_command_ctx *command,
					t_node *arg_node);

/* -------- command_search_and_execution.c ---------------------------------- */
int				command_search_and_execution(t_ctx *c, t_command_ctx *cmd_ctx);

/* -------- execute_non_builtin.c ------------------------------------------- */
int				execute_non_builtin(t_ctx *c, t_command_ctx *cmd_ctx);

/* -------- get_pathname.c -------------------------------------------------- */
int				get_pathname(t_ctx *c, t_command_ctx *cmd_ctx);

/* -------- process_redirection.c ------------------------------------------- */
int				process_redirection(t_ctx *c, t_node *redir_node);

/* -------- ft_split_with_empty.c ------------------------------------------- */
char			**ft_split_with_empty(char const *s, char c);

/* -------- wait_return_status.c ------------------------------------------- */
void		wait_return_status(t_ctx *c);

/* -------- execute_builtin.c ----------------------------------------------- */
int				execute_builtin(t_ctx *c, t_command_ctx *cmd_ctx,
					t_command_function command);

/* -------- execute_builtin_in_subshell.c ----------------------------------- */
int				execute_builtin_in_subshell(t_ctx *c, t_command_ctx *cmd_ctx,
					t_command_function command);

/* -------- env.c ----------------------------------------------------------- */
int				env(t_ctx *c, t_command_ctx *command_ctx);

/* -------- pwd.c ----------------------------------------------------------- */
int				pwd(t_ctx *c, t_command_ctx *command_ctx);
char			*get_pwd(t_ctx *c);

/* -------- parse_token_flow.c ---------------------------------------------- */
bool			get_next_token(t_ctx *c, t_parser_state *p, t_lexer_state *l);

/* -------- parse_input.c --------------------------------------------------- */
t_parser_state	parse_input(t_ctx *c);

/* -------- classify_token.c ------------------------------------------------ */
t_symbol_type	classify_token(t_ctx *c, t_token *token);

/* -------- builtin_exit.c -------------------------------------------------- */
int				builtin_exit(t_ctx *c, t_command_ctx *command_ctx);

/* -------- error_handling.c ------------------------------------------------ */
int				exit_mem_issue(void);
int				msh_error(char *where, char *what, char *why);
int				msh_error_errno(char *where, char *what);
void			fatal(t_ctx *c, char *where, char *why);
int				handle_redirection_error(t_ctx *c, char *filename);
void			child_cleanup_all(t_ctx *c, t_command_ctx *cmd_ctx,
					char **envp);
int				exit_child(t_ctx *c, t_command_ctx *cmd_ctx, char **envp);

/* -------- cd.c ------------------------------------------------------------ */
int				cd(t_ctx *c, t_command_ctx *command_ctx);

/* -------- get_path_canonical_form.c --------------------------------------- */
char			*get_path_canonical_form(char *curpath, size_t len);

/* -------- builtin_export.c ------------------------------------------------ */
int				builtin_export(t_ctx *c, t_command_ctx *command_ctx);

/* -------- unset.c --------------------------------------------------------- */
int				unset(t_ctx *c, t_command_ctx *command_ctx);

/* -------- echo.c ---------------------------------------------------------- */
int				echo(t_ctx *c, t_command_ctx *command_ctx);

/* -------- signal_setup.c -------------------------------------------------- */
int				sig_setup_handler(t_ctx *c);
int				sig_set_default(void);
int				sig_set_interactive(void);

/* -------- signal_helpers.c ------------------------------------------------ */
int				sig_rl_event_hook(void);
bool			sig_consume_sigint(t_ctx *c);
void			sig_reset_sigint(void);

/* -------- ft_close_fd.c --------------------------------------------------- */
void			ft_close_fd(int *fd);

#endif
