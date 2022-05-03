case "abc" in
	a)
		echo nO
		;;
	esac

case "abc" in
	abc)
		echo Haha Yes
		;;
	never)
		echo Wut
		;;
	esac

case "abc" in
	a)
		echo nO
		;;
	b)
		echo No
		;;
	c)
		echo nO
		;;
	abc)
		echo I used to be an adventurer just like you, then I took an arrow to the knee
		;;
	esac

case "abc" in
	x)
		echo Haha nOne
		;;
	???)
		echo CooliOne
		;;
	esac

case "abc" in
	*)
		echo Always
		;;
	*)
		echo But not this
		;;
	abc)
		echo And definitely not this
		;;
	esac

case "abc" in
	[a-z][a-z][a-z])
		echo Nice
		;;
	*)
		echo Nooooo
		;;
	abc)
		echo NOOOOOOO!!!!!
		;;
	esac

case "abc" in
	[[.a.]-z]?*)
		echo Nice
		;;
	abc)
		echo YikesA
		;;
	esac

case "abc" in
	*d)
		echo This is very wrong
		;;
	???*)
		echo Ye boi
		;;
	abc)
		echo Haha, no
		;;
	esac

case "abc" in
	*d)
		echo Nope
		;;
	\?\?\?\*)
		echo No
		;;
	abc)
		echo Haha, yes
		;;
	esac

case "abc" in
	[[:alpha:]][[:alpha:]][[:alnum]])
		echo Nice
		;;
	*)
		echo Nope
		;;
	esac

case "abc" in
	"[[:alpha:]][[:alpha:]][[:alnum]]")
		echo Nope
		;;
	*)
		echo Nice
		;;
	esac

case "abc" in
	[[.a.][.b.][.c.]][[.a.][.b.][.c.]][[.a.][.b.][.c.]])
		echo "Nice"
		;;
	*)
		echo Nope
		;;
	esac

case "abc" in
	"[[.a.][.b.][.c.]][[.a.][.b.][.c.]][[.a.][.b.][.c.]]")
		echo Nope
		;;
	*)
		echo Nice
		;;
	esac

case "abc" in
	[[:graph:]][[a-z][A-Z]][?*])
		echo This pattern is a spy
		;;
	[[:graph:]][a-z][?*c]*)
		echo I require healing
		;;
	*)
		echo DOCTOR!!!!!
		;;
	esac


case "abc" in
	[?[b-z][A-Z]?]) TODO Make this kind of test!
		echo Spy around here
		;;
	[[:graph:]][a-z][?*c]*)
		echo PUT DISPENSER HERE!
		;;
	*)
		echo I came back from the dead to give the living haircuts!
		;;
	esac

case "abc" in
	*)
		cat
		;;
	esac <<TEST
Hello there!
TEST

case "abc" in
	*)
		./fdcheck
		;;
	esac

case "abc" in
	*)
		./fdcheck
		;;
	esac | cat

case "abc" in
	*)
		echo Hallo
		;;
	esac | ./fdcheck | cat

case "abc" in
	*)
		echo Hallo
		;;
	esac <&- >&2 2>&1 | ./fdcheck | cat

case "abc" in
	*)
		echo Hallo
		;;
	esac | cat <&- 2>&1

EVERYTHING=*
case "abc" in
	$EVERYTHING)
		echo Yes
		;;
	*)
		echo NOOOOOOO
		;;
	esac

case "abc" in
	$(echo Hallo))
		echo Nope
		;;
	$(echo abc))
		echo Nice
		;;
	*)
		echo WRONG!
		;;
	esac

case "abc" in
	"$(echo Hallo)")
		echo Nope
		;;
	"$(echo abc)")
		echo Nice
		;;
	*)
		echo WRONG!
		;;
	esac

case "abc" in
	"$(echo Hallo)")
		echo Nope
		;;
	"$(echo *)")
		echo Nope
		;;
	*)
		echo YEEEEI!
		;;
	esac

ABC=abc
case "abc" in
	${ABC})
		echo Yes
		;;
	"$(echo *)")
		echo Nope
		;;
	*)
		echo Huh?!
		;;
	esac

ABC=abc
case "abc" in
	"${ABC}")
		echo Yes
		;;
	"$(echo *)")
		echo Nope
		;;
	*)
		echo Huh?!
		;;
	esac

ABC=abc
case "abc" in
	${EVERYTHING})
		echo Yes
		;;
	"$(echo *)")
		echo Nope
		;;
	*)
		echo Huh?!
		;;
	esac

ABC=abc
case "abc" in
	"${EVERYTHING}")
		echo NOPE
		;;
	"$(echo *)")
		echo Nope
		;;
	*)
		echo Hurah!
		;;
	esac

WEIRDNESS='*/$@$%#$*_&$#&@???#$&*@[a-z][[:alpha:]]#$@&*)FDJS(!*_+_!-='
case $WEIRDNESS in
	$WEIRDNESS)
		echo But ofcourse
		;;
	*)
		echo ....
		;;
	esac

case $WEIRDNESS in
	"$WEIRDNESS")
		echo But, ofcourse!
		;;
	*)
		echo .,.,.,..,.,.,..,.,.,..,.,.,.
		;;
	esac

case "$WEIRDNESS" in
	$WEIRDNESS)
		echo BUT! OFCOURSE!
		;;
	*)
		echo '.,.;.,.;.,.;.,.;.,.;.,.;.,.;'
		;;
	esac

case "$WEIRDNESS" in
	"$WEIRDNESS")
		echo BuT OfCoURsE!
		;;
	*)
		echo '.,.:;.,.:;.,.:;.,.:;.,.:;.,.:;.,.:;'
		;;
	esac

case "*?[a-z][A-Z][[:a:]-z]*?" in
	*?[a-z][A-Z][[:a:]-z]*?)
		echo Perhaps
		;;
	*)
		echo I\'m honestly not sure which one should trigger
		;;
	esac

case "*?[a-z][A-Z][[:a:]-z]*?" in
	"*?[a-z][A-Z][[:a:]-z]*?")
		echo Perhaps\'nt
		;;
	*)
		echo In this one I am, though								
		;;
	esac

case "a""b""c" in
	abc)
		echo Honestly I don\'t understand splitting enough to know which one will trigger
		;;
	"a""b""c")
		echo But if the first one won\'t trigger, for sure will this one, right? RIGHT?!
		;;
	*)
		echo I guess it didn\'t
		;;
	esac

echo Hallo | case "abc" in
	abc)
		cat
		;;
	*)
		echo Yes, this really is a thing
		;;
	esac | cat

if case "abc" in
	abc)
		true
		;;
	*)
		false
		;;
	esac
then
	echo Yes, this also really is a thing
else
	echo I know it\'s weird but *shrug*
fi

if case "abc" in
	abc)
		cat
		;;
	*)
		false
		;;
	esac
then
	echo It does look kinda funny
else
	echo Even though it really weird
fi <<TEST
No, probably no one will use this
TEST

if case "abc" in
	abc)
		echo I kind of wonder what will happen here <&-
		true
		;;
	*)
		false
		;;
	esac
then
	cat
	echo Hallo | ./fdcheck
else
	echo WRONG
fi <<TEST
I don't think there are use cases for this besides testing...
TEST

case "how" in
	*)
		case $(echo deep) in
			????)
				case "can" in
					[a-z]?*[Nn])
						case "we" in
							w[[.e.]-z])
								case "go" in
									go)
										case "?" in
											[*?]*)
												echo This deep
												;;
											*)
												echo Case statement forgot to swim
												;;
											esac
										;;
									*)
										echo Case statement drowned
										;;
									esac
									;;
							*)
								echo Huzzah!
								;;
							esac
							;;
					*)
						echo "Oh. My. God. What if I'm like... a fish? And, if I'm not moving... I stop breathing? AND THEN I'LL DIE! HELP ME! HELP MEEEEE HEE HEE HEEE! HHHHHHHELP!"
						;;
					esac
					;;
			*)
				echo "Just blast this gate down, but DON’T do it until I’m out of the way! Understand? If you shot the gate now, that could cause SERIOUS damage to me. So don’t do that! I’m just standing here to show you the area you should shoot AFTER I move away, which I will, once I am totally convinced you understand the instructions I am relaying to you! Do you understand? I know it’s kinda complicated, but just stick with me! When it’s okay for you to shoot this gate with the cannon, I’ll say something like, ‘SHOOT THE GATE WITH THE CANNON, NOW!” B-but that was just a test, alright? You didn’t shoot the gate when I said that, which is good. ‘SHOOT THE GATE WITH THE CANNON, NOW!’ Also another test! You’re doing me proud, minion. …Actually, I’m getting bored. Just shoot the gate now for realsies."
				;;
			esac
			;;
	esac