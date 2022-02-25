mem_size=300
stride=3
dataptr=0

return=
memory=""
echo "Setting up memory..."
for i in $(seq 1 $mem_size); do
	memory="$memory 0"
done
echo "Done setting up memory!"

# memory="123 456 789"

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

setbyte 2 10
echo $memory

instrs="$(echo "$1" | sed 's/\(.\)/\1 /g')"
count="${#instrs}"
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
	for instr in $instrs
	do
		if [ $instr = "[" ]
		then
			last_cond=$cur
			cur_depth=$(($cur_depth + 1))
		elif [ $instr = "]" ]
		then
		fi

		if [ $cur -eq $pc ] && [ $cond_fj -eq 0 ] && [ $cond_bj -eq 0 ]
		then
			break
		elif [ $instr = "]" ] && [ $cond_fj -eq 1 ]
		then
			cur="$((cur + 1))"
			pc="$cur"
			cond_fj=0
			break
		elif [ $instr = "[" ] && [ $cond_bj -eq 1 ] && [ $cur_depth -eq $b_depth ]
		then
			pc=$(($last_cond - 1))
			echo "New pc:$pc"
			instr=""
			cond_bj=0
			break
		else
			cur=$((cur + 1))
		fi
	done
	case $instr in
		">")
			echo -n ">"
			dataptr=$(($dataptr + 1))
			if [ $dataptr -ge $(($mem_size * $stride)) ]
			then
				echo "Out of bounds (upper)"
				exit
			fi
			;;
		"<")
			echo -n "<"
			dataptr=$(($dataptr - 1))
			if [ $dataptr -lt 0 ]
			then
				echo "Out of bounds (lower)"
				exit
			fi
			;;
		"+")
			echo -n "+"
			getbyte $dataptr
			byte=$return

			byte=$(($byte + 1))
			if [ $byte -gt 255 ]
			then
				echo "Data overflow"
				exit
			fi
			setbyte $dataptr $byte
			;;
		"-")
			echo -n "-"
			getbyte $dataptr
			byte=$return

			byte=$(($byte - 1))
			if [ $byte -lt 0 ]
			then
				echo "Data underflow"
				exit
			fi
			setbyte $dataptr $byte
			;;
		".")
			echo -n "."
			getbyte $dataptr
			printf "%c" $return
			;;
		",")
			echo -n ","
			read -n 1 $val
			val=$(printf "%d" "'$val")
			setbyte $dataptr $val
			;;
		"[")
			echo -n "["
			getbyte $dataptr
			byte=$return

			echo $byte
			if [ $byte -eq 0 ]
			then
				depth=$(($depth + 1))
				echo "Took forward jump"
				cond_fj=1
				continue
			fi
			;;
		"]")
			echo -n "]"
			getbyte $dataptr
			byte=$return

			echo "{$dataptr=$byte}"

			if [ $byte -ne 0 ]
			then
				echo "Took backward jump pc:$pc"
				cond_bj=1
				b_depth=$depth
				continue
			else
				depth=$(($depth - 1))
			fi
			;;
	esac
	pc=$(($pc + 1))
	if [ $pc -ge $count ]
	then
		break
	fi
done
echo "Done"
