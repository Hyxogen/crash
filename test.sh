printf \"\"\ -a
printf \ \"%s\" . "${@-a}" .
printf \\n
printf \"\"\ :-a
printf \ \"%s\" . "${@:-a}" .
printf \\n
printf \"\"\ +a
printf \ \"%s\" . "${@+a}" .
printf \\n
printf \"\"\ :+a
printf \ \"%s\" . "${@:+a}" .
printf \\n
