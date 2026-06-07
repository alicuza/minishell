#!/bin/bash

. ./test/base_test.sh

test_basename=$(basename "$0")
test_basename="${test_basename%.sh}"

compare_bash_shni $test_basename  "pwd"