TEST=""
printf \ \"%s\" . "${@-a}" . ${TEST-"$@"} .
printf \\n

printf \ \"%s\" . "${@:-a}" . ${TEST:-"$@"} .
printf \\n

printf \ \"%s\" . "${@+a}" . ${TEST+"$@"} .
printf \\n

printf \ \"%s\" . "${@:+a}" . ${TEST:+"$@"} .
printf \\n
