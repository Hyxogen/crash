display_usage() {
	cat <<END >&2
$progname [-bdgpstwy] [-h] [-e eyes] [-f cowfile]
	[-l] [-n] [-T tongue] [-W wrapcolumn] [message]
END
	exit
}