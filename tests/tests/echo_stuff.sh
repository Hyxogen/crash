echo | cat -e
echo -n | cat -e

echo | cat - e
echo -n | cat - e

echo "I’ve been thinking. When life gives you lemons? Don’t make lemonade. Make life take the lemons back! Get mad! I don’t want your damn lemons! What am I supposed to do with these? Demand to see life’s manager! Make life rue the day it thought is could give me lemons! Do you know who I am? I’m the man who’s going to burn your house down! With the lemons! I’m going to get my engineers to invent a combustible lemon that burns your house down!" | cat -e
echo -n "I’ve been thinking. When life gives you lemons? Don’t make lemonade. Make life take the lemons back! Get mad! I don’t want your damn lemons! What am I supposed to do with these? Demand to see life’s manager! Make life rue the day it thought is could give me lemons! Do you know who I am? I’m the man who’s going to burn your house down! With the lemons! I’m going to get my engineers to invent a combustible lemon that burns your house down!" | cat -e

echo $(echo "I’ve been thinking. When life gives you lemons? Don’t make lemonade. Make life take the lemons back! Get mad! I don’t want your damn lemons! What am I supposed to do with these? Demand to see life’s manager! Make life rue the day it thought is could give me lemons! Do you know who I am? I’m the man who’s going to burn your house down! With the lemons! I’m going to get my engineers to invent a combustible lemon that burns your house down!") | cat -e
echo -n $(echo "I’ve been thinking. When life gives you lemons? Don’t make lemonade. Make life take the lemons back! Get mad! I don’t want your damn lemons! What am I supposed to do with these? Demand to see life’s manager! Make life rue the day it thought is could give me lemons! Do you know who I am? I’m the man who’s going to burn your house down! With the lemons! I’m going to get my engineers to invent a combustible lemon that burns your house down!") | cat -e

echo $(echo ) | cat -e
echo -n $(echo ) | cat -e

echo $(echo -n) | cat -e
echo -n $(echo -n) | cat -e

echo $NOTHING | cat -e
echo -n $NOTHING | cat -e

echo ${NOTHING} | cat -e
echo -n ${NOTHING} | cat -e

echo "$NOTHING" | cat -e
echo -n "$NOTHING" | cat -e

echo "${NOTHING}" | cat -e
echo -n "${NOTHING}" | cat -e

BIBLE='I’ve been thinking. When life gives you lemons? Don’t make lemonade. Make life take the lemons back! Get mad! I don’t want your damn lemons! What am I supposed to do with these? Demand to see life’s manager! Make life rue the day it thought is could give me lemons! Do you know who I am? I’m the man who’s going to burn your house down! With the lemons! I’m going to get my engineers to invent a combustible lemon that burns your house down!'

echo $BIBLE | cat -e
echo -n $BIBLE | cat -e

echo "$BIBLE" | cat -e
echo -n "$BIBLE" | cat -e

echo "${BIBLE}" | cat -e
echo -n "${BIBLE}" | cat -e

echo "whomst''d've''ly''yaint''nt''ed''ies''s''y''es" | cat -e
echo -n "whomst''d've''ly''yaint''nt''ed''ies''s''y''es" | cat -e

echo "\x22Hello World!\x22" | cat -e
echo -n "\x22Hello World!\x22" | cat -e

echo \x22Hello World!\x22 | cat -e
echo -n \x22Hello World!\x22 | cat -e

echo "\?$*’MaRViN’*$?\"" | cat -e
echo -n "\?$*’MaRViN’*$?\"" | cat -e

echo これも出来るのか？
echo -n これも出来るのか？

BEES=$(curl https://gist.githubusercontent.com/ElliotGluck/64b0b814293c09999f765e265aaa2ba1/raw/79f24f9f87654d7ec7c2f6ba83e927852cdbf9a5/gistfile1.txt)

echo $BEES | cat -e
echo -n $BEES | cat -e

WHUT=$(curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0xff_assigned_including-unprintable-asis_unseparated.txt)

echo $WHUT
echo -n $WHUT

echo "$WHUT"
echo -n "$WHUT"

WHUT=$(curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0xfff_assigned_including-unprintable-asis_unseparated.txt)

echo $WHUT
echo -n $WHUT

echo "$WHUT"
echo -n "$WHUT"

# WHUT=$(curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/raw/master/UTF-8_sequence_unseparated/utf8_sequence_0-0xffff_including-unassigned_including-unprintable-replaced_unseparated.txt)

# echo $WHUT
# echo -n $WHUT

# echo "$WHUT"
# echo -n "$WHUT"

# DO NOT TRY THIS TEST
# YOUR PC WILL FREEZE
# YOU HAVE BEEN WARNED
# WHUT=$(curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt)
# NOW IS THE TIME TO TURN BACK
# echo $WHUT
# echo -n WHUT
