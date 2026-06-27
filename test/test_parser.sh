. test/helpers.sh

test_simple_echo()
{
	local input expected

	input="$(cat <<- \eof
		echo hello
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
