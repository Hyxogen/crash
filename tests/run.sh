#!/bin/sh
dir=$(dirname $0)

run_test() {
	bash_out=${1%.sh}_bash.txt
	crash_out=${1%.sh}_crash.txt
	diff_out=${1%.sh}.diff
	bash_err=${1%.sh}_err_bash.txt
	crash_err=${1%.sh}_err_crash.txt
	diff_err=${1%.sh}_err.diff
	bash $1 > $bash_out 2> $bash_err
	$dir/../minishell $1 > $crash_out 2> $crash_err
	diff $bash_out $crash_out > $diff_out
	diff $bash_err $crash_err > $diff_err
	printf "\033[37m$(basename $1) "
	if [ -s $diff_out ]; then
		printf "\033[31mKO\033[0m "
	else
		printf "\033[32mOK\033[0m "
	fi
	if [ -s $diff_err ]; then
		printf "\033[33mKO\033[0m\n"
	else
		printf "\033[32mOK\033[0m\n"
	fi
}

if [ $# -gt 0 ]; then
	run_test $dir/tests/$1.sh
else
	for file in $dir/tests/*.sh; do
		run_test $file
	done
fi