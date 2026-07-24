. test/helpers.sh

# ------------------------------------------------------------------------------
# Parser token tests
# ------------------------------------------------------------------------------
#
# Each test sends a line of input through the `--no_exec --scope=tokens`
# pipeline and compares the resulting token stream against the expected block.
#
# Dimensions used in per-test annotations:
#
#   CMD     command name form(s)                bare | abs | rel
#
#   ARGC    argument count per command          0 | 1 | N
#
#   REDIR   kind                                in | heredoc | out | append
#           + properties                        with_space | no_space | no_escape
#
#   EXPAND  type                                question | var
#           + position                          bare | suffix | enclosed
#
#   QUOTES  type                                double | single | unclosed
#           + position                          bare | prefix
#
#   OP      pipeline/chain operator(s)          pipe | and_if | or_if
#
#   OTHER   other test attributes               comment
#
# Within a brace, `|` separates sub-attributes of a single entity (e.g.
# REDIR={out|no_space} means kind=out, properties=no_space).
#
# Commas within braces separate multiple entities of the same dimension
# (CMD, ARGC, REDIR, OP) — e.g. CMD={bare, bare},
# REDIR={in|with_space, out|with_space}.
#
# ------------------------------------------------------------------------------

# ----- Command forms ----------------------------------------------------------

# CMD={bare}  ARGC={1}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={abs}  ARGC={1}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={rel}  ARGC={1}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# ----- Arguments --------------------------------------------------------------

# CMD={bare}  ARGC={3}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={abs}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={rel}  ARGC={3}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
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

# ----- Redirects -------------------------------------------------------------

# CMD={bare}  ARGC={1}  REDIR={in|with_space}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={1}  REDIR={heredoc|with_space|no_escape}  EXPAND={}  QUOTES={}  OP={}
test_redir_heredoc()
{
	local input expected
	input="$(cat <<- \eof
		cat << EOF
			hello
			EOF
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(cat)
			SYM_OPERATOR(<<)
			SYM_TOKEN(EOF)
			SYM_TOKEN(hello)
			SYM_TOKEN(EOF)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD={bare}  ARGC={1}  REDIR={out|with_space}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={1}  REDIR={append|with_space}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={1}  REDIR={in|with_space, out|with_space}  EXPAND={}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={1}  REDIR={out|no_space}  EXPAND={}  QUOTES={}  OP={}
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

# ----- Operators --------------------------------------------------------------

# CMD={bare, bare}  ARGC={1, 1}  REDIR={}  EXPAND={}  QUOTES={}  OP={pipe}
test_op_pipe()
{
	local input expected
	input="$(cat <<- \eof
		ls | wc
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(ls)
			SYM_OPERATOR(|)
			SYM_TOKEN(wc)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD={bare, bare}  ARGC={2, 2}  REDIR={}  EXPAND={}  QUOTES={}  OP={and_if}
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

# CMD={bare, bare}  ARGC={2, 2}  REDIR={}  EXPAND={}  QUOTES={}  OP={or_if}
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

# CMD={bare, bare, bare}  ARGC={1, 1, 1}  REDIR={}  EXPAND={}  QUOTES={}  OP={pipe}
test_op_pipe_chain()
{
	local input expected
	input="$(cat <<- \eof
		ls | wc | cat
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(ls)
			SYM_OPERATOR(|)
			SYM_TOKEN(wc)
			SYM_OPERATOR(|)
			SYM_TOKEN(cat)
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD={bare, bare, bare}  ARGC={1, 1, 1}  REDIR={}  EXPAND={}  QUOTES={}  OP={pipe, and_if}
test_op_mixed()
{
	local input expected
	input="$(cat <<- \eof
		ls | wc && env
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(ls)
			SYM_OPERATOR(|)
			SYM_TOKEN(wc)
			SYM_OPERATOR(&&)
			SYM_TOKEN(env)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ----- Whitespace -------------------------------------------------------------

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
test_whitespace_tab()
{
	local input expected
	input="$(cat <<- \eof
		echo	hello
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(echo)
			SYM_TOKEN(hello)
		eof
	)"
	assert_shell "$input" "$expected"
}

# ----- Flags ------------------------------------------------------------------

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={double|bare}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={bare}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={enclosed}  QUOTES={double|bare}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={double|bare}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={question|bare}  QUOTES={}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={double|prefix}  OP={}
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={suffix}  QUOTES={}  OP={}
test_flag_mid_word_expansion()
{
	local input expected
	input="$(cat <<- \eof
		echo love$HOME
		eof
	)"
	expected="$(cat <<- \eof
		TOKENS
			SYM_TOKEN(echo)
			SYM_TOKEN(love$HOME) LEX_HAS_EXPANSION
		eof
	)"
	assert_shell "$input" "$expected"
}

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={single|bare}  OP={}
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

# ----- Miscellaneous cases ----------------------------------------------------

# CMD={}  ARGC={0}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
test_empty_input()
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

# CMD={}  ARGC={0}  REDIR={}  EXPAND={}  QUOTES={}  OP={}
test_only_spaces()
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={double|unclosed}  OP={}
test_unclosed_double_quote()
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={single|unclosed}  OP={}
test_unclosed_single_quote()
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

# CMD={}  ARGC={}  REDIR={}  EXPAND={}  QUOTES={}  OP={}  OTHER={comment|bare}
test_comment_only()
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

# CMD={bare}  ARGC={2}  REDIR={}  EXPAND={}  QUOTES={}  OP={}  OTHER={comment|suffix}
test_comment_after_cmd()
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
