len=$1

first=0
second=1

for i in $(seq 1 $len); do
	echo "$first"
	res="$((first + second))"
	first=$second
	second=$res
done