# hello() {
# 	echo "there"
# }

# hello
# echo $?

# yes() {
# 	false
# }

# yes
# echo $?

# hello() {
# 	echo "haha no"
# }

# hello

# whatactuallyisthis() {
# 	echo Honstely why...
# } | cat
# echo $?
# whatactuallyisthis
# whatactuallyisthis
# whatactuallyisthis

# hereboi() {
# 	cat
# } <<TEST
# Oh, for God's - they told me that if I ever turned this flashlight on, I would die!
# They told me that about everything!
# I don't even know why they bother giving me this stuff if they didn't want me to use it; it's pointless! Mad!
# TEST

# hereboi
# hereboi
# hereboi
# hereboi <&-
# hereboi <<TEST
# Shut up wheatley
# TEST

# hereboi() {
# 	cat
# } <<TEST
# Excellent! 
# You’re a predator and these tests are your prey. 
# Speaking of which, I was researching sharks for an upcoming test. 
# Do you know who else murders people who are only trying to help them?
# Did you guess “sharks”? Because that’s wrong.
# The correct answer is “nobody”.
# Nobody but you are that pointlessly cruel.
# TEST >&2 2>&1 | tr a-z A-Z | otherboi() {
# 	cat
# 	echo Yes, this actually is a thing
# } <<TEST
# The Enrichment Center promises to always provide a safe testing environment.
# In dangerous testing environments, the Enrichment Center promises to always provide useful advice.
# For instance: the floor here will kill you.
# Try to avoid it.
# TEST

# otherboi
# echo $?
# hereboi
# echo $?
# hereboi
# echo $?
# otherboi

# thing() {
# 	echo Thing
# 	canyouseeme() {
# 		echo Can you see me?
# 		probablynot() {
# 			echo Probably not
# 			youknowthatthisisstupid() {
# 				echo You know that this is stupid
# 			}
# 		}
# 	}
# }

# youknowthatthisisstupid
# echo $?
# probablynot
# echo $?
# canyouseeme
# echo $?
# thing
# echo $?
# youknowthatthisisstupid
# echo $?
# probablynot
# echo $?
# canyouseeme
# echo $?
# echo thing
# echo $?
# youknowthatthisisstupid
# echo $?
# probablynot
# echo $?
# canyouseeme
# echo $?
# thing
# echo $?
# youknowthatthisisstupid
# echo $?
# probablynot
# echo $?
# canyouseeme
# echo $?
# thing
# echo $?

func1() {
	cat
	func2() {
		cat
		func3() {
			cat
			func4() {
				cat
			} <<-TEXT
			Goodbye!
			TEXT
		} <<-TEXT
		Thank you for participating in that Aperture Science Enrichment activity.
		TEXT
	} <<-TEXT
	Rest assured, that there is absolutely no chance of a dangerous equipment malfunction prior to your victory candescence.
	TEXT
}<<TEXT
All Aperture technologies remain safely operational up to 4000 degrees kelvin.
TEXT

func1
func2
func3
func4
func1
func2
func3
func4

yes() {
	echo *Teleports behind you*
	yes() {
		echo "Nothing personal, kid"
		yes() {
			echo なに？！
		}
	}
}

yes
echo $?
yes
echo $?
yes
echo $?
yes
echo $?

true
whatevenisthereturnvalueofafunction() {
	false
}
echo $?

true
echo $?
im_running_out_of_function_names() {
	echo Hallo
} | false | true | false
echo $?

wait_it_actually_executes_this_pipe_sequence() {
	echo Yes
} | echo It does
wait_it_actually_executes_this_pipe_sequence

if true;
then
	space() {
		echo Hallo
	}
fi <<SPACE
SPAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACE!!
SPACE

space