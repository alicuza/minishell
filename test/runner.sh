#!/bin/bash

run()
{
	local result=0;
	for test_file in test/test_builtin_*; do
		./$test_file || result=1
	done;

	. test/test_parser.sh
	for func in $(awk '/^test_/ {print $1}' test/test_parser.sh); do
		"${func%()}" || result=1
	done

	return $result;
}

eval run
