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
		echo Yes
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
		echo Nope
		;;
	???*)
		echo Ye boi
		;;
	abc)
		echo Haha, no
		;;
	esac