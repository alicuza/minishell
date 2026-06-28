_This project has been created as part of the 42 curriculum by nribakov, sancuta._

# minishell

## Description

`minishell` is a miniature shell with a limited feature set. A shell is a program that manages the interaction between **user** and **system**. In a REPL (read-eval-print loop), the shell prompts the user for input, interprets the commands according to the command language syntax, and handles the output from the system.

---

## Table of Contents

- [Description](#description)
- [Instructions](#instructions)
  - [Compilation](#compilation)
  - [Usage](#usage)
  - [Functionality](#functionality)
- [Known Limitations](#known-limitations)
- [Algorithm and Data Structure](#algorithm-and-data-structure)
  - [Architecture Overview](#architecture-overview)
  - [Core Data Structure](#core-data-structure)
  - [Parsing](#parsing)
  - [Execution](#execution)
  - [Signals](#signals)
  - [Error Handling and Cleanup](#error-handling-and-cleanup)
- [Deviation from bash](#deviation-from-bash)
  - [Grammar (mandatory)](#grammar-mandatory)
  - [Grammar (bonus)](#grammar-bonus)
- [Resources](#resources)
  - [Documentation and References](#documentation-and-references)
    - [Special Thanks To](#special-thanks-to)
  - [AI Usage](#ai-usage)

---

## Instructions

### Compilation

- `make` or `make all` — compiles `minishell`;
- `make clean` — removes object and dependency files;
- `make fclean` — additionally removes the binary, libs, docs, and logs;
- `make re` — recompiles the entire project from scratch;
- `make debug` — compiles with `-g -DDEBUG` for debugging;
- `make run` — builds and runs `./minishell`;
- `make run-debug` — builds and runs `./minishell-debug`;
- `make test` — builds both release and debug binaries, then runs the test suite;
- `make doc` — generates Doxygen man pages in `docs/` (requires `doxygen`);
- `make compile_flags.txt` — generates a clangd-compatible compilation database;

**Options:**
- `make [target] ARENA_SIZE=N` — overrides the arena initial capacity (default `64`);

**Dependencies:**
- libft (bundled)

### Usage

```bash
./minishell
```

### Functionality

- readline-based REPL with colored prompt (`user@hostname:cwd[exitcode]shni$ `)
- POSIX-conformant tokenizer (10 rules, quotes, expansions, operators)
- shift-only LALR(1) parser (reductions planned)
- builtins: `env`, `pwd`, `exit` (partial)
- arena-based memory management (no per-allocation tracking)
- interactive and non-interactive modes
- debug mode with `--scope=<flags>` and `--no_exec`

---

## Known Limitations

- **Shift-only parser:** the parser stacks tokens (`t_symbol`) but does not perform grammar reductions. No non-terminal nodes (pipeline, command, etc.) are created. `t_rule` table and `t_reduce` function pointers are defined but never populated.
- **No external commands:** there is no `fork()`/`execve()` loop. Only the builtins `env`, `pwd`, and `exit` are wired; `exit` does not terminate the shell.
- **No pipelines, redirections, subshells, or chaining:** `|`, `<`, `>`, `<<`, `>>`, `&&`, `||`, `(`, `)` are lexed but never acted upon.
- **No expansion:** `$NAME`, `$?`, and quoted strings are flagged but never expanded or quote-removed. No field splitting.
- **No heredoc:** `<<` is tokenized but the heredoc body reader is not implemented.
- **No signal handling:** `<signal.h>` is included but no handlers are registered. `SIGINT` and `SIGQUIT` are ignored by default.
- **Incomplete builtins:** only 3 of the 7 required builtins are stubbed. `echo`, `cd`, `export`, `unset` are declared in `builtin.h` but not wired in `map_to_command()`.
- **`t_cmd` arena:** `AT_CMD` is declared in `t_ctx` but never initialized — no command structs are created.

---

## Algorithm and Data Structure

### Architecture Overview

The program state lives in a single `t_ctx` struct passed by pointer through every stage. Memory is managed through typed arenas — there is no per-allocation tracking or explicit `free` for individual tokens.

```
t_ctx
├── t_env          env            — linked list of environment key/value pairs
├── t_arena[4]     arena          — typed arenas (AT_PROMPT, AT_STRING, AT_STACK, AT_CMD*)
├── char          *read_line      — readline-allocated input (freed per iteration)
├── int            return_status  — exit code of the last command
├── bool           is_interactive — true when STDIN is a tty
└── (debug)        scope, no_exec — debug flags (only with -DDEBUG)
```

The parsing pipeline is split into three phases:

1. **Lexer** — character-by-character POSIX tokenizer. Reads from `read_line`, produces `t_token` lookaheads, copies token bodies into `AT_STRING`.
2. **Parser** — shift-only LALR(1) parser. Stacks `t_symbol` nodes on `AT_STACK` with linked-list links (`prev_symbol`). Grammar reductions are defined in `types.h` (`t_rule`, `t_reduce`, 48 rules) but not yet implemented.
3. **Token Processor** — flat walk of the symbol stack. Dispatches `SYM_TOKEN` nodes through `map_to_command()`. Only `env`, `pwd`, `exit` are currently recognized.

Arena types use different strides: `AT_PROMPT` and `AT_STRING` are character buffers (stride=1), `AT_STACK` has stride=`sizeof(t_symbol)`, `AT_CMD` is reserved but not initialized.


### Core Data Structure

```c
typedef struct s_ctx
{
	t_env       env;                 // linked list of key/value pairs
	t_arena     arena[AT_COUNT];     // 4 arenas (PROMPT, STRING, STACK, CMD [unused])
	char       *read_line;           // readline-allocated input string
	int         return_status;       // exit status of last command
	bool        is_interactive;      // true if STDIN is a tty
# ifdef DEBUG
	uint8_t     scope;               // debug scope flags
	bool        no_exec;             // skip execution when true
# endif
}	t_ctx;

typedef struct s_symbol
{
	uint64_t      offset;            // offset into AT_STRING arena
	uint64_t      prev_symbol;       // arena index of logical predecessor
	uint64_t      next_frame;        // subshell chain (future use)
	uint64_t      next_pipeline;     // pipeline chain (future use)
	uint64_t      next_redir;        // command redirect chain (future use)
	uint64_t      next_arg;          // command arg chain (future use)
	uint64_t      next_cmd;          // pipeline level (future use)
	uint32_t      entry_state;       // parser state before this symbol
	uint32_t      flags;             // LEX_HAS_EXPANSION | LEX_HAS_QUOTES
	t_symbol_type type;              // SYM_TOKEN, SYM_OPERATOR, ...
}	t_symbol;

typedef struct s_parser_state
{
	uint32_t  cur_state;             // current parser state (always 0 — no reductions)
	uint64_t  arena_idx;             // physical index of most recently shifted symbol
	uint64_t  stack_idx;             // logical index (currently == arena_idx)
	t_token   lookahead;             // the current lookahead token
	uint8_t   flags;                 // PARSE_HERE_PENDING | PARSE_DONE (unused)
}	t_parser_state;

typedef struct s_lexer_state
{
	t_slice       token;             // position+length of token being built
	uint64_t      char_idx;          // index into read_line
	t_symbol_type type;              // SYM_TOKEN or SYM_OPERATOR
	uint8_t       flags;             // LEX_HAS_EXPANSION | LEX_HAS_QUOTES | ...
}	t_lexer_state;

typedef struct s_token
{
	uint64_t      offset;            // offset into AT_STRING arena
	uint32_t      flags;             // same flags as lexer_state
	t_symbol_type type;              // SYM_TOKEN, SYM_OPERATOR, etc.
}	t_token;
```

### User Input

Input is handled by a readline wrapper in `srcs/input.c`:

- **Interactive mode:** `isatty(STDIN_FILENO)` is checked once at startup. The prompt is built in the `AT_PROMPT` arena and includes color, user, hostname, cwd, and exit status: `\033[38;5;40muser@hostname:cwd[0]shni$ \033[0m`. Escape sequences are wrapped in `\001`/`\002` to prevent readline redisplay corruption.
- **Continuation prompt:** when `is_continuation` is true, a simple `"> "` is used (hardcoded, not `PS2`).
- **Non-interactive mode:** `prompt` is set to `NULL` and `rl_outstream` is redirected to `stderr` so readline does not mirror input to stdout.
- **History:** non-empty lines are added to readline history via `add_history()` in interactive mode. History is not persisted between sessions.

### Parsing

Parsing follows a two-stage pipeline: **tokenization** (lexer) and **syntax analysis** (parser). The lexer reads characters from `read_line` and produces delimited tokens. The parser shifts those tokens onto a stack as `t_symbol` nodes.

#### Tokenization

The lexer in `srcs/lexer/` implements the POSIX token recognition rules from Section 2.3 of the Shell & Utilities volume. It is a character-by-character state machine:

```
Lexer state tracks:
  token:      t_slice {pos, len}     current extent in read_line
  char_idx:   uint64_t               current read position
  type:       SYM_TOKEN / SYM_OPERATOR
  flags:      LEX_HAS_EXPANSION      token contains $ expansion
              LEX_HAS_QUOTES         token contains ' or " quotes
              LEX_IS_BUILDING        currently accumulating token
              LEX_IS_DELIMITED       token is complete, ready to shift
              LEX_NEEDS_INPUT        consumed all input, need more
```

The rules are applied in order (see POSIX 2.3 Token Recognition):

1. **EOI/EOF** — delimit current token if one exists
2. **Operator continuation** — if `prev_char` + `cur_char` form a longer operator (e.g. `<` + `<` → `<<`), extend the token
3. **Operator break** — if `prev_char` was an operator but `cur_char` cannot extend it, delimit the operator token
4. **Quotes** — `'` or `"` starts a quoted region; characters are added verbatim until the matching close quote
5. **Expansion** — `$` starts a variable reference; following valid `name_chars` are consumed
6. **Operator start** — unquoted `|`, `<`, `>`, `&`, `;`, `(`, `)` start a new operator token; delimit any existing token first
7. **Blank** — ` ` or `\t` delimits an existing token and is discarded
8. **Word continuation** — any other character extends a word token
9. **Comment** — `#` (when not quoted) discards until end of line (not required, optionally compiled)
10. **Default** — start a new word token

On delimiting, the token body is copied into `AT_STRING` arena and a `t_token` is produced as the lookahead.

#### Syntax Analysis

The parser in `srcs/parser/` is a **shift-only LALR(1) parser**. The main loop in `parse_input()`:

1. Create a zeroed `t_parser_state` and `t_lexer_state`
2. Set `parse.arena_idx = 1` (index 0 is the sentinel guard element)
3. Loop:
   - Call `get_lookahead()` — runs the lexer until a token is delimited
   - When a token is ready (`LEX_IS_DELIMITED`), call `shift_symbol()`
   - `shift_symbol()` allocates a `t_symbol` on `AT_STACK`, sets its offset, type, flags, entry_state (always 0), and `prev_symbol` pointing to the previous symbol
   - Update `stack_idx` and `arena_idx`
   - Repeat until EOF

No reductions are performed. The grammar rules (`t_rule[RULE_COUNT]`) and reduce function pointers (`t_reduce`) are defined in `types.h` but never populated. The parser creates a flat linked list of symbols — no non-terminal nodes (pipeline, command, etc.) are synthesized.

#### Grammar
*see [2.10 Shell Grammar](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_10)*

**Shell Grammar Lexical Conventions**
Lexing happens immediately following the `token` being delimited.

```
1.	if
		`cur_token` is `operator`
	do
		identify as corresponding `token_id`
```

```
2.	if	// we skip this
		`cur_token` is only `digits`
		&& `delimiter` is `<` or `>`
	do
		identify as `IO_NUMBER`
```

```
3.	if	// we skip this
		`cur_token` is `{LOCATION}`
		&& `delimiter` is `<` or `>`
	do
		identify as `IO_LOCATION`
```

```
4.	do
		identify as `TOKEN`
```

```ebnf
/* -------------------------------------------------------
       Selection of Grammar Symbols used in Minishell
   ------------------------------------------------------- */

%token  WORD				/* ".*" */
%token  ASSIGNMENT_WORD		/* 'NAME[=]WORD' */
%token  NAME				/* 'WORD("[a-zA-Z_][a-zA-Z0-9_]*")' */
%token  NEWLINE				/* '\n'*/
%token  IO_NUMBER			/* 'WORD("[0-9].*")'[GREAT|LESS]*/

%token	AND_IF	OR_IF	DLESS	DGREAT	LESS	GREAT	OPAR	CPAR
/*		'&&'	'||'	'<<'	'>>'	'<'		'>'		'('		')'		*/
```

**Shell Grammar Rules**

```
1. (rule 2 - redirection to or from `filename`)
	1.1. tilde expansion
	1.2. parameter/variable expansion
	1.3. quote removal		// actually 5th expansion, but we don't handle command substitution or arithmetic expansion
	1.4. filename expansion
	1.5. word splitting		// if more than one word, bash errors with: `bash: $var: ambiguous redirect`
```

```
2. (rule 3 - redirection from `here-doc`) 
	Quote removal of `WORD` after `DLESS` to determine `here-doc` `delimiter`
```

```
3. (rule 7 - assignment preceding command name)
	//TODO: continue writing
```

**documentation**
- [2.7 Redirections](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_07)
- [2.10.2 Shell Grammar Rules](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_10_02)
- [3.45 Blank Character](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap03.html#tag_03_45)

### Bash Specific Terms

**bash grammar**
the following chars don't have an explicit token they get assigned to in the grammar.

```
	'&'	'|'	'('	')'	'<'	'>'
```

- being single `char` tokens, they can just represent themselves?

#### metacharacters

*see [2. Definitions - metacharacter](https://www.gnu.org/software/bash/manual/bash.html#index-metacharacter)*
- characters that separate words when unquoted.

```
'|', '&', ';', '(', ')', '<', '>'
' ', '\t', '\n'
```

#### operators

*see [2. Definitions - operator](https://www.gnu.org/software/bash/manual/bash.html#index-operator_002c-shell)*
- `operator`s contain at least one unquoted `metacharacter`

#### blanks
*see [2. Definitions](https://www.gnu.org/software/bash/manual/bash.html#Definitions)*

```
' ', '\t'
```

#### control operators
*see [2. Definitions - control operator](https://www.gnu.org/software/bash/manual/bash.html#index-control-operator)*

```
'||', '&&', '(', ')'
'&', ';', ';;', ';&', ';;&', '|', '|&' - not implemented
```

#### redirection operators
*see [3.6 Redirections](https://www.gnu.org/software/bash/manual/bash.html#Redirections)*

```
'<', '>', '>>'
'<<' // here-doc
```

#### name
*see [2. Definitions - name](https://www.gnu.org/software/bash/manual/bash.html#index-name)*
- `word`s that conform to the following format: `[a-zA-Z_][a-zA-Z0-9_]`

### Execution

Execution is limited to a flat walk of the symbol stack in `token_processor.c`:

- `exec_stack()` traverses the linked list of `t_symbol` nodes from top to bottom via `prev_symbol`.
- For each `SYM_TOKEN` node, `process_token()` extracts the token string from `AT_STRING` and dispatches through `map_to_command()`.
- `map_to_command()` compares against known builtins by name. Currently recognizes: `env`, `pwd`, `exit`.
- Unknown commands return 0 silently — no `fork()`/`execve()`, no PATH lookup, no child process.
- No pipelines (`pipe()`), no redirections (`dup2()`), no subshells, no `&&`/`||` chaining.

### Signals

No signal handlers are currently registered. `<signal.h>` is included in `minishell.h` and the research phase covered signal handling (readline's `rl_set_signals()`, async-safe patterns), but wiring them to the parser/execution loop is not yet done. `SIGINT` and `SIGQUIT` are left at their system defaults.

### Error Handling and Cleanup

Cleanup is arena-based and centralized in `main.c`:

- On exit, `cleanup()` calls `arena_free()` on `AT_STRING`, `AT_STACK`, and `AT_PROMPT` arenas, then `free_env()` to clear the environment list.
- Per iteration, `arena_reset()` reinitializes the `AT_STRING` and `AT_STACK` arenas (frees and reallocates buffers), and `free(c->read_line)` releases the readline-allocated input.
- The prompt arena (`AT_PROMPT`) is reused within an iteration via `arena_clear()` (resets offset, keeps buffer).
- No systematic error propagation — most functions return `int` but callers often ignore the value. `errno`-based retry is only used in the prompt `getcwd()` logic.

## Deviation from bash

- `ASSIGNMENT_WORD` - bash potentially unnecessay, because we can handle this construct in `export` as a builtin with string parsing
- `NAME` - not needed as separate token type, might still be required for some things
- `IO_NUMBER` - fd-specific redirects like `2>` are not mentioned by the subject, but according to Renés notes regarding the campus consensus it is not required
- `;`, `#`, `!`, `\`, `~` - not required, but interesting to consider
  - `~` - for the dynamic prompt
  - `#` - for comments, because that seems useful

---

### Grammar (mandatory)

```ebnf
input       : command_list NEWLINE
            | NEWLINE
            ;

command_list: pipeline
            ;

pipeline    : pipeline PIPE command
            | command
            ;

command     : simple_command
            ;

simple_command
            : prefix WORD suffix
            | WORD suffix
            | WORD
            | prefix WORD
            | prefix
            ;

prefix      : redirection
            | prefix redirection
            ;

suffix      : WORD
            | redirection
            | suffix WORD
            | suffix redirection
            ;

redirection : LESS WORD
            | GREAT WORD
            | DGREAT WORD
            | DLESS WORD
            ;
```

**Precedence:** `|` is left-associative (`a | b | c` → `(a | b) | c`).

**Key invariant (mandatory):** a `simple_command` is complete when the next token is `NEWLINE` or `PIPE`. Execution happens at that boundary.

---

### Grammar (bonus) 

```ebnf
command_list: command_list AND_IF pipeline
            | command_list OR_IF pipeline
            | pipeline
            ;

pipeline    : pipeline PIPE command
            | command
            ;

command     : simple_command
            | OPAR command_list CPAR
            ;
```

**Precedence:** `|` binds tighter than `&&`/`||`. So `cmd1 && cmd2 | cmd3` parses as `cmd1 && (cmd2 | cmd3)`.

**Key invariant (bonus):** also `AND_IF`, `OR_IF`, and `CPAR` terminate a `simple_command`.

---

## Resources

### Documentation and References

#### Special Thanks To

### AI Usage

AI tools were used mainly for interactive rubber ducking, and searching for information.

