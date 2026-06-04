#!/bin/bash

run() {
local result=0;
for test_file in test/test_*; do
  ./$test_file || result=1
done;
return $result;
}

eval run

