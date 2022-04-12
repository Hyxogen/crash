# TODO redirect stderr back to stdout
./fdcheck 1>&2 | ./fdcheck 1>&2 | ./fdcheck

echo builtin_echo >&2 2>&1 | tr a-z A-Z
/bin/echo bin_echo >&2 2>&1 | tr a-z A-Z