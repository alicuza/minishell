. test/helpers.sh

# ============================================================================
# Parser token tests
# ============================================================================
#
# Each test sends a line of input through the `--no_exec --scope=tokens`
# pipeline and compares the resulting token stream against the expected block.
#
# Dimensions used in per-test annotations:
#   CMD    command name form             bare | abs | rel
#   ARGS   argument count                0 | 1 | N
#   REDIR  redirect kind                 none | < | << | > | >>
#   FLAGS  token flags, space separated  none | quotes | expansion | both
#   OP     pipeline/chain operator       none | '|' | '&&' | '||'
#   NCMD   number of commands            1 | 2 | N
#
# ============================================================================

# ===== Command forms ========================================================

# CMD=bare  ARGS=0  REDIR=none  FLAGS=none  OP=none
test_cmd_bare()
{
	local input expected
	input="$(cat <<- \eof
		ls
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(ls)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD=abs  ARGS=0  REDIR=none  FLAGS=none  OP=none
test_cmd_abs()
{
	local input expected
	input="$(cat <<- \eof
		/bin/pwd
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(/bin/pwd)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD=rel  ARGS=0  REDIR=none  FLAGS=none  OP=none
test_cmd_rel()
{
	local input expected
	input="$(cat <<- \eof
		./minishell
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(./minishell)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Arguments ============================================================

# CMD=bare  ARGS=2  REDIR=none  FLAGS=none  OP=none
test_args_bare()
{
	local input expected
	input="$(cat <<- \eof
		echo hello world
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN(hello)
		SYM_TOKEN(world)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD=abs  ARGS=1  REDIR=none  FLAGS=none  OP=none
test_args_abs()
{
	local input expected
	input="$(cat <<- \eof
		/bin/echo hello
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(/bin/echo)
		SYM_TOKEN(hello)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD=rel  ARGS=2  REDIR=none  FLAGS=none  OP=none
test_args_rel()
{
	local input expected
	input="$(cat <<- \eof
		./script.sh -a foo
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(./script.sh)
		SYM_TOKEN(-a)
		SYM_TOKEN(foo)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Redirects ============================================================

# REDIR=<  FLAGS=none  OP=none
test_redir_input()
{
	local input expected
	input="$(cat <<- \eof
		cat < file
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(cat)
		SYM_OPERATOR(<)
		SYM_TOKEN(file)
		eof
	)"
	assert_shell "$input" "$expected"
}

# REDIR=<<  FLAGS=none  OP=none
test_redir_heredoc()
{
	local input expected
	input="$(cat <<- \eof
		cat << EOF
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(cat)
		SYM_OPERATOR(<<)
		SYM_TOKEN(EOF)
		eof
	)"
	assert_shell "$input" "$expected"
}

# REDIR=>  FLAGS=none  OP=none
test_redir_output()
{
	local input expected
	input="$(cat <<- \eof
		echo > file
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_OPERATOR(>)
		SYM_TOKEN(file)
		eof
	)"
	assert_shell "$input" "$expected"
}

# REDIR=>>  FLAGS=none  OP=none
test_redir_append()
{
	local input expected
	input="$(cat <<- \eof
		echo >> file
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_OPERATOR(>>)
		SYM_TOKEN(file)
		eof
	)"
	assert_shell "$input" "$expected"
}

# REDIR=< then >  FLAGS=none  OP=none
test_redir_multiple()
{
	local input expected
	input="$(cat <<- \eof
		cat < in > out
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(cat)
		SYM_OPERATOR(<)
		SYM_TOKEN(in)
		SYM_OPERATOR(>)
		SYM_TOKEN(out)
		eof
	)"
	assert_shell "$input" "$expected"
}

# REDIR=>  no surrounding whitespace — operator split mid-word
test_redir_adjacent()
{
	local input expected
	input="$(cat <<- \eof
		echo>file
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_OPERATOR(>)
		SYM_TOKEN(file)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Operators ============================================================

# OP=|  NCMD=2
test_op_pipe()
{
	local input expected
	input="$(cat <<- \eof
		cmd1 | cmd2
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(cmd1)
		SYM_OPERATOR(|)
		SYM_TOKEN(cmd2)
		eof
	)"
	assert_shell "$input" "$expected"
}

# OP=&&  NCMD=2
test_op_and()
{
	local input expected
	input="$(cat <<- \eof
		echo a && echo b
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN(a)
		SYM_OPERATOR(&&)
		SYM_TOKEN(echo)
		SYM_TOKEN(b)
		eof
	)"
	assert_shell "$input" "$expected"
}

# OP=||  NCMD=2
test_op_or()
{
	local input expected
	input="$(cat <<- \eof
		echo a || echo b
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN(a)
		SYM_OPERATOR(||)
		SYM_TOKEN(echo)
		SYM_TOKEN(b)
		eof
	)"
	assert_shell "$input" "$expected"
}

# OP=|  NCMD=3   chained pipes
test_op_pipe_chain()
{
	local input expected
	input="$(cat <<- \eof
		a | b | c
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(a)
		SYM_OPERATOR(|)
		SYM_TOKEN(b)
		SYM_OPERATOR(|)
		SYM_TOKEN(c)
		eof
	)"
	assert_shell "$input" "$expected"
}

# OP=| then &&  NCMD=3   mixed operators
test_op_mixed()
{
	local input expected
	input="$(cat <<- \eof
		a | b && c
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(a)
		SYM_OPERATOR(|)
		SYM_TOKEN(b)
		SYM_OPERATOR(&&)
		SYM_TOKEN(c)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Whitespace ===========================================================

# separator=TAB  (BLANK_SET includes \t but never exercised)
test_whitespace_tab()
{
	local input expected
	input=$'echo\thello'
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN(hello)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Flags ================================================================

# FLAGS=quotes  (double-quoted word)
test_flag_quotes()
{
	local input expected
	input="$(cat <<- \eof
		echo "hello"
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN("hello") LEX_HAS_QUOTES
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=expansion  ($HOME expansion)
test_flag_expansion()
{
	local input expected
	input="$(cat <<- \eof
		echo $HOME
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN($HOME) LEX_HAS_EXPANSION
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=both  (quoted expansion — only LEX_HAS_QUOTES set currently; inner $ is
#              consumed raw by the quote-pair loop, not the expansion rule)
test_flag_both()
{
	local input expected
	input="$(cat <<- \eof
		echo "$HOME"
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN("$HOME") LEX_HAS_QUOTES LEX_HAS_EXPANSION
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=quotes  (empty quoted string)
test_flag_empty_quotes()
{
	local input expected
	input="$(cat <<- \eof
		echo ""
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN("") LEX_HAS_QUOTES
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=expansion  ($? special parameter)
test_flag_special_param()
{
	local input expected
	input="$(cat <<- \eof
		echo $?
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN($?) LEX_HAS_EXPANSION
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=quotes  (quoted-token concat: "foo"bar — one token; quotes produce no
#                word split, LEX_HAS_QUOTES marks the whole combined token)
test_flag_quote_concat()
{
	local input expected
	input="$(cat <<- \eof
		echo "foo"bar
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN("foo"bar) LEX_HAS_QUOTES
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=expansion  ($VAR mid-word, no split)
test_flag_mid_word_expansion()
{
	local input expected
	input="$(cat <<- \eof
		echo$HOME
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo$HOME) LEX_HAS_EXPANSION
		eof
	)"
	assert_shell "$input" "$expected"
}

# FLAGS=quotes  (single-quoted word — same code path as double quotes)
test_flag_single_quotes()
{
	local input expected
	input="$(cat <<- \eof
		echo 'hello'
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN('hello') LEX_HAS_QUOTES
		eof
	)"
	assert_shell "$input" "$expected"
}

# ===== Edge cases ===========================================================

test_edge_empty_input()
{
	local input expected
	input="$(cat <<- \eof

		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		eof
	)"
	assert_shell "$input" "$expected"
}

test_edge_only_spaces()
{
	local input expected
	input="$(cat <<- \eof
		   
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		eof
	)"
	assert_shell "$input" "$expected"
}

test_edge_unclosed_dquote()
{
	local input expected
	input="$(cat <<- \eof
		echo "foo
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN("foo)
		eof
	)"
	assert_shell "$input" "$expected"
}

test_edge_unclosed_squote()
{
	local input expected
	input="$(cat <<- \eof
		echo 'foo
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN('foo)
		eof
	)"
	assert_shell "$input" "$expected"
}

test_edge_comment_only()
{
	local input expected
	input="$(cat <<- \eof
		# comment
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		eof
	)"
	assert_shell "$input" "$expected"
}

test_edge_comment_after_cmd()
{
	local input expected
	input="$(cat <<- \eof
		echo hi # comment
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
		SYM_TOKEN(echo)
		SYM_TOKEN(hi)
		eof
	)"
	assert_shell "$input" "$expected"
}
