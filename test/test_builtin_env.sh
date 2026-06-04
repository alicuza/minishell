#!/bin/bash
function clean_data(){
	sed -i '/SHLVL=/d' "$shni_result"
	sed -i '/SHLVL=/d' "$bash_result"
	sed -i '/_=/d' "$shni_result"
	sed -i '/_=/d' "$bash_result"

	sort -V "$bash_result" > "$bash_result.tmp" && mv "$bash_result.tmp" "$bash_result"
	sort -V "$shni_result" > "$shni_result.tmp" && mv "$shni_result.tmp" "$shni_result"
}

. ./test/base_test.sh

test_basename=$(basename "$0")
test_basename="${test_basename%.sh}"

compare_bash_shni $test_basename  "env" clean_data

