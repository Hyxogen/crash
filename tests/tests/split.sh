IFS=": "

TMP=""
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n

TMP=" "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=":"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n
printf \\n

TMP=" : "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP="::"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=" : : "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP="a:b"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=" a : b "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=":a:b:"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=" : a : b : "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP="a::b"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=" a : : b "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP="::a::b::"
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n

TMP=" : : a : : b : : "
printf \"%s\"\  $TMP$TMP.$TMP""$TMP
printf \\n
printf \"%s\"\  "$TMP""$TMP"."$TMP""""$TMP"
printf \\n
printf \"%s\"\  " $TMP "" $TMP "." $TMP """" $TMP "
printf \\n\\n
