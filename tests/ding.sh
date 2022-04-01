IFS=:
while read a b; do
	echo $a
	<<-eof
	Hello
	eof
done </etc/passwd