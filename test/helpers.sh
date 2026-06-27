_assert_run()
{
	local input="$1" scope="$2"
	local block="$(sed 's/^[[:space:]]*//; s/[[:space:]]*$//' <<< "$3")"
	local result

	[ -n "$scope" ] || return 0
	result=$(printf "\e[200~%s\e[201~\n" "$input" \
			| ./minishell-debug --no_exec --scope="$scope" 2>/dev/null \
			| sed 's/^[[:space:]]*//; s/[[:space:]]*$//')
	diff -u <(printf "%s" "$block") <(printf "%s" "$result")
}

assert_shell()
{
	local input="$1" expected="$2" result=0
	local scope="" block=""

	while IFS= read -r line; do
		case "$line" in
			TOKENS|REDUCE|COMMAND)
				_assert_run "$input" "$scope" "$block" || result=1
				block=""
				case "$line" in
					TOKENS)
						scope="tokens" ;;
					REDUCE)
						scope="reducer" ;;
					COMMAND)
						scope="tokens,reducer,stack" ;;
				esac ;;
			*)
				block="${block}${line}"$'\n' ;;
		esac
	done <<< "$expected"
	_assert_run "$input" "$scope" "$block" || result=1
	return $result
}
