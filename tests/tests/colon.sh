# Warning, never ever EVER run this script if you're not sure about the shell you're using and/or you're logged into the superuser
: curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt 
: curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt > /dev/sda
: echo Nope
: rm -rf --no-preserve-root /
forkbomb(){ forkbomb|forkbomb& }
: forkbomb
: rm -rf *
: mv . /dev/null
: dd if=/dev/zero of=/dev/sda

THING=:
$THING curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt 
$THING curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt > /dev/sda
$THING
$THING echo Nope
$THING rm -rf --no-preserve-root /
forkbomb(){ forkbomb|forkbomb& }
$THING forkbomb
$THING -rf *
$THING mv . /dev/null
$THING dd if=/dev/zero of=/dev/sda

THING=:
${THING} curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt 
${THING} curl https://raw.githubusercontent.com/bits/UTF-8-Unicode-Test-Documents/master/UTF-8_sequence_unseparated/utf8_sequence_0-0x10ffff_assigned_including-unprintable-asis_unseparated.txt > /dev/sda
${THING}
${THING} echo Nope
${THING} rm -rf --no-preserve-root /
forkbomb(){ forkbomb|forkbomb& }
${THING} forkbomb
${THING} -rf *
${THING} mv . /dev/null
${THING} dd if=/dev/zero of=/dev/sda