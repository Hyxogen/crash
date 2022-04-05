TEST=""
printf \"\"\ -a
printf \ \"%s\" . "${@-a}" . ${TEST-"$@"} .
printf \\n

printf \"\"\ :-a
printf \ \"%s\" . "${@:-a}" . ${TEST:-"$@"} .
printf \\n

printf \"\"\ +a
printf \ \"%s\" . "${@+a}" . ${TEST+"$@"} .
printf \\n

printf \"\"\ :+a
printf \ \"%s\" . "${@:+a}" . ${TEST:+"$@"} .
printf \\n
