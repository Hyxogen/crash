#!/bin/sh
dir=$(dirname $0)
for file in $dir/tests/*.sh; do
	bash_out=${file%.sh}_bash.txt
	crash_out=${file%.sh}_crash.txt
	diff_out=${file%.sh}.diff
	/bin/sh $file > $bash_out
	$dir/../minishell $file > $crash_out
	diff $bash_out $crash_out > $diff_out
	if [ -s $diff_out ]; then
		printf "\033[37m$(basename $file) \033[31mKO\033[0m\n"
	else
		printf "\033[37m$(basename $file) \033[32mOK\033[0m\n"
	fi
done