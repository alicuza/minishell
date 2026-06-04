#!/bin/bash

function compare_bash_shni(){
echo Running: $1

#Create file
file_name_prefix="./log/${1}"
date=$(date '+%Y-%m-%d_%H.%M.%S')
bash_result="${file_name_prefix}_bash_${date}"
shni_result="${file_name_prefix}_shni_${date}"
if [ ! -d "./log" ]; then
  mkdir "./log"
fi
$(touch $bash_result);
$(touch $shni_result);


#Execute
echo "$2" | bash > $bash_result;
echo "$2" | ./minishell > $shni_result;

#Trim prompt line
sed -i '1d' "$shni_result"
sed -i '$d' "$shni_result"

eval $3

#Check result
difference="$(diff -u "$bash_result" "$shni_result")"
if [ -z "$difference" ]; then
  echo OK
#   rm $bash_result
#   rm $shni_result
  return 0
else
  echo KO: test $1 failed, here is diff:
  echo "$difference"
  return 1
fi
}
