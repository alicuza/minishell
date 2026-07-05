#!/bin/bash

. ./test/base_test.sh

test_basename=$(basename "$0")
test_basename="${test_basename%.sh}"

compare_bash_shni $test_basename  "cd \n pwd"
compare_bash_shni $test_basename  "cd ."
compare_bash_shni $test_basename  "cd .."
compare_bash_shni $test_basename  "cd /"
compare_bash_shni $test_basename  "cd /home"
compare_bash_shni $test_basename  "cd /home/"
compare_bash_shni $test_basename  "cd /home//"
compare_bash_shni $test_basename  "cd /home///"
compare_bash_shni $test_basename  "cd /home////"
compare_bash_shni $test_basename  "cd /home/."
compare_bash_shni $test_basename  "cd /home/./"
compare_bash_shni $test_basename  "cd /home/.."
compare_bash_shni $test_basename  "cd /home/../"
compare_bash_shni $test_basename  "cd /usr/include/../"
compare_bash_shni $test_basename  "cd /usr/include/../../"
compare_bash_shni $test_basename  "cd /../"
compare_bash_shni $test_basename  "cd /../../"
