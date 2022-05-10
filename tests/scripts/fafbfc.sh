echo 'put() { printf \\$(printf %o $1); }'
echo 'get() { read -n 1; printf %d \'\''$REPLY; }'
for cmd in $(sed 's/\(.\)/\1 /g' "$1"); do
	case $cmd in
		">") echo ': $((bf_ptr+=1))';;
		"<") echo ': $((bf_ptr-=1))';;
		"+") echo ': $((bf_$((bf_ptr))+=1))';;
		"-") echo ': $((bf_$((bf_ptr))-=1))';;
		".") echo 'put $((bf_$((bf_ptr))))';;
		",") echo ': $((bf_$((bf_ptr))=$(get)))';;
		"[") echo 'while [ $((bf_$((bf_ptr)))) -ne 0 ]; do :';;
		"]") echo 'done';;
	esac
done
