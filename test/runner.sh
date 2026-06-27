#!/bin/bash

_run_functions()
{
	for func in $(awk '/^test_/ {print $1}' "$1"); do
		echo "Running: ${func%()}"
		"${func%()}" || result=1
	done
}

run()
{
	local result=0;
	for test_file in test/test_*; do
		case $test_file in
			*builtin*)
				./$test_file || result=1 ;;
			*)
				. ./$test_file
				_run_functions $test_file ;;
		esac;
	done;

	return $result;
}

eval run
