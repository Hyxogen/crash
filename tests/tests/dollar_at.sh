BAR=""
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set ""
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set "a"
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set "" ""
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set "a" ""
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set "" "a"
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n

set "a" "a"
printf \"%s\"\  ${@-a} . ${@-} . ${@-""} . "${@-a}" . "${@-}" . "${@-""}" . ${FOO-"$@"} . ${BAR-"$@"} . "${FOO-$@}" . "${BAR-$@}" . "${FOO-"$@"}" . "${BAR-"$@"}"
printf \\n
printf \"%s\"\  ${@:-a} . ${@:-} . ${@:-""} . "${@:-a}" . "${@:-}" . "${@:-""}" . ${FOO:-"$@"} . ${BAR:-"$@"} . "${FOO:-$@}" . "${BAR:-$@}" . "${FOO:-"$@"}" . "${BAR:-"$@"}"
printf \\n
printf \"%s\"\  ${@+a} . ${@+} . ${@+""} . "${@+a}" . "${@+}" . "${@+""}" . ${FOO+"$@"} . ${BAR+"$@"} . "${FOO+$@}" . "${BAR+$@}" . "${FOO+"$@"}" . "${BAR+"$@"}"
printf \\n
printf \"%s\"\  ${@:+a} . ${@:+} . ${@:+""} . "${@:+a}" . "${@:+}" . "${@:+""}" . ${FOO:+"$@"} . ${BAR:+"$@"} . "${FOO:+$@}" . "${BAR:+$@}" . "${FOO:+"$@"}" . "${BAR:+"$@"}"
printf \\n\\n
