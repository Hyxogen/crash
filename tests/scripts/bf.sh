mem_size=50
stride=3
dataptr=0

stack=""
stack_ptr=-1

return=
memory=""
echo "Setting up memory..."
for i in $(seq 1 $mem_size); do
	memory="$memory 0"
done
echo "Done setting up memory!"

# memory="123 456 789"

print_current() {
	debugprint "$str"
	index=0
	for i in $(seq 1 $pc)
	do
		debugnprint " "
	done
	debugprint "^"
}

getbyte() {
	index=0
	for b in $memory; do
		if [ $index -eq $1 ]
		then
			return="$b"
			break
		fi
		index=$(($index + 1))
	done
}

setbyte() {
	val=$2
	return=0
	front=""
	index=0
	new_mem=""
	for b in $memory; do
		if [ $index -eq $1 ]
		then
			b=$val
		fi
		new_mem="${new_mem} ${b}"
		index=$(($index + 1))
	done
	memory=$new_mem
}

stack_push() {
	stack="$stack $1"
	stack_ptr=$(($stack_ptr + 1))
	return=1
}

stack_pop() {
	index=0
	new_stack=""
	if [ "$stack" = "" ]
	then
		print_current
		echo "Pop on empty stack"
		exit
	fi

	for i in $stack
	do
		if [ $index -eq $stack_ptr ]
		then
			return=$i
		else
			new_stack="$new_stack $i"
		fi
		index=$(($index + 1))
	done
	stack=$new_stack
	stack_ptr=$(($stack_ptr - 1))
}

stack_top() {
	index=0
	return=0
	for i in $stack
	do
		if [ $index -eq $stack_ptr ]
		then
			return=$i
		fi
		index=$(($index + 1))
	done
}

debugprint() {
	if [ 0 -eq 1 ]
	then
		echo "$1"
	fi
}

debugnprint() {
	if [ 0 -eq 1 ]
	then
		echo -n "$1"
	fi
}

stack_clear() {
	stack=""
}

echo $memory

str=$1
str="$(echo "$str" | tr -d ' ')"
echo $str
instrs="$(echo "$str" | sed 's/\(.\{1\}\)/\1 /g')"
count="${#str}"
history=""
cond_fj=0
cond_bj=0
b_depth=0
last_cond=0
depth=0
pc=0
echo "$instrs"
while true
do
	# getbyte $dataptr
	# echo -n "[$dataptr]:{$return}"
	cur=0
	cur_depth=0
	debugprint "pc:$pc"
	for instr in $instrs
	do
		# if [ $instr = "[" ]
		# then
		# 	push "$cur"
		# 	last_cond=$cur
		# 	cur_depth=$(($cur_depth + 1))
		# elif [ $instr = "]" ]
		# then

		# fi

		if [ $cur -eq $pc ] && [ $cond_fj -eq 0 ] && [ $cond_bj -eq 0 ]
		then
			break
		# elif [ $instr = "]" ] && [ $cond_fj -eq 1 ] && [ $cur -ge $pc ]
		# then
		# 	pc="$cur"
		# 	cur="$((cur + 1))"
		# 	cond_fj=0
		# 	instr=""
		# 	break
		# # elif [ $instr = "[" ] && [ $cond_bj -eq 1 ] && [ $cur_depth -eq $b_depth ]
		# then
		# 	pc=$(($last_cond - 1))
		# 	echo "New pc:$pc"
		# 	instr=""
		# 	cond_bj=0
		# 	break
		else
			cur=$((cur + 1))
		fi
	done
	debugprint "instr:$instr"
	print_current
	if [ $cond_fj -eq 0 ]
	then
		case $instr in
			">")
				debugnprint ">"
				dataptr=$((($dataptr + 1) % $mem_size))
				# if [ $dataptr -ge $(($mem_size * $stride)) ]
				# then
				# 	echo "Out of bounds (upper)"
				# 	exit
				# fi
				;;
			"<")
				debugnprint "<"
				dataptr=$((($dataptr - 1) % $mem_size))
				# if [ $dataptr -lt 0 ]
				# then
				# 	echo "Out of bounds (lower)"
				# 	exit
				# fi
				;;
			"+")
				debugnprint "+"
				getbyte $dataptr
				byte=$return

				byte=$(($byte + 1))
				# if [ $byte -gt 255 ]
				# then
				# 	echo "Data overflow"
				# 	exit
				# fi
				setbyte $dataptr $byte
				;;
			"-")
				debugnprint "-"
				getbyte $dataptr
				byte=$return

				byte=$(($byte - 1))
				# if [ $byte -lt 0 ]
				# then
				# 	echo "Data underflow"
				# 	exit
				# fi
				setbyte $dataptr $byte
				;;
			".")
				debugnprint "."
				getbyte $dataptr
				# echo "printval:$return"
				printf "\\$(printf '%03o' "$return")"
				;;
			",")
				debugnprint ","
				val=
				read -n 1 $val
				val=$(printf "%d" "'$val")
				setbyte $dataptr $val
				;;
			"[")
				debugnprint "["
				getbyte $dataptr
				byte=$return

				if [ $byte -eq 0 ]
				then
					depth=$(($depth + 1))
					debugprint "Took forward jump"
					cond_fj=$stack_ptr
				else
					stack_push $pc
					debugprint "Pushed $pc onto the stack"
				fi
				;;
			"]")
				debugnprint "]"
				getbyte $dataptr
				byte=$return

				debugprint "{$dataptr=$byte}"

				if [ $byte -ne 0 ]
				then
					debugprint "$stack"
					stack_top
					pc=$return
					debugprint "Took backward jump pc:$pc"
					b_depth=$depth
				else
					stack_pop
					debugprint "Popped stack"
					depth=$(($depth - 1))
				fi
				;;
		esac
	else
		case $instr in
		"[")
			stack_push=$pc
			;;
		"]")
			stack_pop
			if [ $stack_ptr -eq $depth ]
			then
				cond_fj=0
			fi
			;;
		esac
	fi
	pc=$(($pc + 1))
	if [ $pc -ge $count ]
	then
		break
	fi
done
echo "Done"
echo

echo "Memory dump"
echo "$memory"