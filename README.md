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

- `make` or `make all`: compiles `minishell`
- `make clean`: removes object and dependency files
- `make fclean`: additionally removes the binary and libs
- `make re`: recompiles the entire project from scratch
- `make debug`: compiles with the `-g` flag for debugging
- // TODO: complete the description with debugging funtionality
- // TODO: also add descriptions about the linking of binaries

**Options:**
- `make [target] ARENA_SIZE=N`: overrides the arena initial capacity (default `64`)
// TODO: doesn't recompile with with e different `ARENA_SIZE`

**Dependencies:**
- libft (bundled)

### Usage

```bash
./minishell
```

### Functionality

```bash
```

---

## Known Limitations

---

## Algorithm and Data Structure

### Architecture Overview

- stack based parsing, similar to how bash does it, but with hard coded rules.


### Core Data Structure

```c
```

### User Input



### Parsing

**possible implementation**

```
1. call readline with PS1 prompt
2. tokenize the readline:
2. a. if a token exists and is delimited -> add token string to the string arena, if WORD token -> append `\0`
3. process the token (attempt to reduce token sequence according to the shell grammar; order up for debate, need to consult posix):
3. a. if `cur_token` not empty -> create a command struct with the token type, if there is no command struct that the token can be reduced to; track the "grammar structure" type as the type of this command struct; it gets delimited when the next token is a `control operator` (need to still figure out how this works with parentheses)
3. b. if processing reduces token to `io_here` (a token sequence of `<<` and `WORD`):
3. b. i. tokenize rest of line -> do 2.
3. b. ii. free read_line
3. b. iii. call readline with PS2 prompt
3. b. iv. if the current read_line doesn't contain only `delimiter\n` -> create WORD token and copy the whole read_line into the string arena
3. b. v. else  -> delimit `here-doc` WORD token, if it exists (maybe the conditions in d. and f. could be switched)
3. b. vi. if other `io_here` in saved tokens -> repeat from b.
3. b. free read_line
3. c. process each saved token
3. d. if `current_token` is empty: there are no more tokens to be processed, current command structure gets delimited, if it exists -> sent to execution
4. if there are no more command structures -> wait for the status of the execution
```

relevant rules from [2.10.2 Shell Grammar Rules](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_10_02)
1. command name: `token` -> `WORD`
2. redirection to/from filename: expansions according to [2.7. Redirection](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html#tag_19_07)
3. redirection from here-doc: quote removal on `WORD` -> `delimiter`
~~4. case state termination: not relevant~~
~~5. `NAME` in `for`: not relevant~~
~~6. third word of `for` (`in`, `do`) and `case` (`in`): not relevant~~
7. assignment preceeding command name:
7. a. not relevant
7. b. worth looking into
~~8. `NAME` in function: not relevant~~
~~9. Body of function: not relevant~~

#### Token Recognition

Input is read in terms of lines in 2 different circumstances:

**here-doc processing**

	step 1.
	if
		`io_here` has been "recognized" (returned)
	do
		search for next `\n`-token: corresponding `here-doc` starts on the next line
		&& non-`\n`-tokens get saved for processing after `here-doc` finished parsing

	step 2.
	if
		`\n`-token found
	do
		start `here-doc` on the next line

	step 3.
	if
		`io-here` was among tokens saved
	do
		start corresponding `here-doc` on the line after the `delimiter\n`

	step 4. // TODO: not sure what exactly is meant with processing further, applying the grammar rules?
	if
		there are saved tokens
	do
		process them further

*see [2.7.4 Here-Document](https://pubs.opengroup.org/onlinepubs/9799919799/)*
- the `here-doc` is treated as a single word starting after the first `\n`
- continues until a line containing only the `delimiter` and a `\n`, no `blank`s
- if there is another `here-doc`, it starts immediately after the `delimiter\n`
- expansion happens during `redirection evaluation`
- expansion of `here-doc`s has the same rules as `"`-expansion
  - except for the `"`, which has no special meaning in a `here-doc`
- the order of `here-doc`s corresponds to the order of `io_here` tokens

**ordinary token recognition**
apply the first applicable rule from the list:

	rule 1.
	if
		`cur_char` is `EOI`/`EOF`
	do
		delimit `cur_token`, if it exists

	rule 2.
	if
		`prev_char` is part of `operator`
		&& `cur_char` is unquoted
		&& `cur_char` can be used with the `prev_char` to form an `operator`
	do
		add `cur_char` to the `cur_token`

	rule 3.
	if
		`prev_char` is part of `operator`
		&& `cur_char` cannot be used with the `prev_char` to form an `operator`
	do
		delimit the `cur_token`

	rule 4.
	if
		`cur_char` is a `quote_char`(`'`, `"`)
	do
		add `cur_char` to the `cur_token`
		&& add following `char`s to the `cur_token` unmodified until the closing `quote_char` was found
		&& DO NOT DELIMIT `cur_token`

	rule 5.
	if
		`cur_char` is unquoted
		&& `cur_char` is beginning of variable expansion (`$`)
	do
		add `cur_char` to the `cur_token`
		&& add following `char`s to the `cur_token` unmodified while valid `name_chars`

	rule 6.
	if
		`cur_char` is unquoted
		&& `cur_char` is start of an `operator`
	do	
		delimit `cur_token` if it exists

	rule 7.
	if
		`cur_char` is unquoted
		&& `cur_char` is `blank` (` `, `\t`)
	do
		delimit `cur_token`
		&& discrad `cur_char`

	rule 8.
	if
		`prev_char` is part of `word_token`
	do	
		add `cur_char` to the `cur_token`/`word_token`

	rule 9.	// we skip rule 9 for now
		`cur_char` is `comment_char` (`#`)
	do
		discard `cur_char`
		&& discrad `chars` until `\n`

	rule 10.
	do
		`cur_char` is used as the start of a new `word_token`

Once delimited, a token gets lexed according to the Shell Grammar.

"In situations where the shell parses its input as a program, once a `complete_command` has been recognized by the grammar (see 2.10 Shell Grammar), the `complete_command` shall be executed before the next `complete_command` is tokenized and parsed."

Tokens that are empty after delimiting get discarded.

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

### Signals

### Error Handling and Cleanup

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

