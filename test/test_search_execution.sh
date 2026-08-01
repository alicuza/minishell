#!/bin/bash

. ./test/base_test.sh

test_basename=$(basename "$0")
test_basename="${test_basename%.sh}"

compare_bash_shni $test_basename  "$(cat <<- \eof
		/usr/bin/ls
		echo $_
		eof
	)"

compare_bash_shni $test_basename  "$(cat <<- \eof
		./minishell
		echo $SHLVL
		eof
	)"
