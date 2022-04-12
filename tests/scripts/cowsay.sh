#!/bin/sh

# TODO:
# arithmetic expansions
# super extra special utilities

export eyes
export tongue
export full
export thoughts

cowfile="default.cow"
eyes="oo"
tongue="  "
wrapcolumn=40
progname="$(basename $0)"
cowpath="${COWPATH:-cows}"

wrap() {
	width=$1
	line="$2"
	shift 2
	for word in $*; do
		if [ $((${#line} + ${#word})) -ge $width ]; then
			echo "$line"
			line="$word"
		else
			line="$line $word"
		fi
	done
	[ -n "$line" ] && echo "$line"
}

list_cowfiles() {
	for d in $(IFS=: echo $cowpath); do
		echo "Cow files in $d:"
		files=
		for file in $(ls "$d"); do
			if [ "${file%.cow}" != "$file" ]; then
				files="$files ${file%.cow}"
			fi
		done
		wrap 80 $files
	done
	exit
}

display_usage() {
	cat <<END >&2
$progname [-bdgpstwy] [-h] [-e eyes] [-f cowfile]
	[-l] [-n] [-T tongue] [-W wrapcolumn] [message]
END
	exit
}

maxlength() {
	max=0
	for line in $*; do
		if [ ${#line} -gt $max ]; then
			max=${#line}
		fi
	done
	echo $max
}
	
repeat() {
	if [ $1 -gt 0 ]; then
		for i in $(seq 1 $1); do
			printf "$2"
		done
	fi
}

construct_balloon() {
	count=$(echo "$2" | wc -l)
	if [ "${1#*think}" != "$0" ]; then
		thoughts="o"
		border="( ) ( ) ( )"
	elif [ $count -lt 2 ]; then
		thoughts="\\"
		border="< >"
	else
		thoughts="\\"
		border="/ \\ \\ / | |"
	fi
	echo "$2" | (
		IFS="$(printf "\n\b")"
		len=$(maxlength $2)
		index=0
		echo " $(repeat $(($len + 2)) "_")"
		while read line; do
			if [ $index -eq 0 ]; then
				offset=0
			elif [ $index -eq $(($count - 1)) ]; then
				offset=4
			else
				offset=8
			fi
			left=$(echo "$border" | head -c$(($offset + 1)) | tail -c1)
			middle="$line$(repeat $(($len - ${#line})) " ")"
			right=$(echo "$border" | head -c$(($offset + 3)) | tail -c1)
			index=$(($index + 1))
			echo "$left $middle $right"
		done
		echo " $(repeat $(($len + 2)) "-")"
	)
}

construct_face() {
	if [ $borg ]; then eyes="=="; fi
	if [ $dead ]; then eyes="xx" tongue="U "; fi
	if [ $greedy ]; then eyes="\$\$"; fi
	if [ $paranoid ]; then eyes="@@"; fi
	if [ $stoned ]; then eyes="**" tongue="U "; fi
	if [ $tired ]; then eyes="--"; fi
	if [ $wired ]; then eyes="OO"; fi
	if [ $young ]; then eyes=".."; fi
}

get_cow() {
	perl <<-END
		my \$thoughts = \$ENV{'thoughts'};
		my \$eyes = \$ENV{'eyes'};
		my \$tongue = \$ENV{'tongue'};
		my \$eye = '0';
		open my \$file, '<', \$ENV{'full'} or die \$!;
		my \$file_content = do { local \$/; <\$file> };
		eval \$file_content;
		print \$the_cow;
	END
}

while getopts bde:f:ghlLnNpstT:wW:y name; do
	case $name in
		b) borg=1;;
		d) dead=1;;
		g) greedy=1;;
		p) paranoid=1;;
		s) stoned=1;;
		t) tired=1;;
		w) wired=1;;
		y) young=1;;
		e) eyes=$(echo $OPTARG | head -c2);;
		T) tongue=$(echo $OPTARG | head -c2);;
		h) display_usage;;
		l) list_cowfiles;;
		f) cowfile="$OPTARG";;
		n) nowrap=1;;
		W) wrapcolumn=$OPTARG;;
		?) exit 1;;
	esac
done

shift $(($OPTIND - 1))
message="${*:-`cat`}"
if [ ! $nowrap ]; then
	message=$(wrap $wrapcolumn $message)
else
	message=$(echo "$message" | expand -t 8)
fi

if [ "${cowfile#*/}" != "$cowfile" ]; then
	full=$cowfile
else
	for d in $(IFS=: echo $cowpath); do
		if [ -f "$d/$cowfile" ]; then
			full="$d/$cowfile"
			break
		elif [ -f "$d/$cowfile.cow" ]; then
			full="$d/$cowfile.cow"
			break
		fi
	done
fi
if [ ! -f "$full" ]; then
	echo "$0: Could not find $cowfile cowfile!"
	exit 1
fi

construct_balloon "$0" "$message"
construct_face
get_cow