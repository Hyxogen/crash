for i in One Two Three Four
do echo $i
done

thing=Yes
for thing in manners maketh man
do
	echo $thing
done

GROCERIES="Butter sugar tea flower toothpaste"
for item in $GROCERIES
do
	echo $item
done

CL4PTP="Ow! Guys, you don’t HAVE to beat me up – we can talk this through, right? Here, I’ll do it for you! \"Hey Claptrap, how are you?” “Oh, I’m fine – I kinda wish you wouldn’t beat me up, though.” “Why?” “Cause it really hurts!” “ You make a good point, Claptrap, but beating you up makes US feel really good!” “I know, guys, but it makes ME feel really bad!” “But Claptrap, you’re a robot -- are you even capable of feeling pain?” “Well, uh, no, I guess I’m not.” “So, pummeling you makes us feel good, and DOESN’T hurt you, there’s no harm in us continuing to do it, right?” ...Actually, now that I’ve talked it all out. I think you guys have the moral high ground, here. Pummel away!"

for word in $CL4PTP
do
	echo $word
done
	echo $word

for word in "$CL4PTP"
do
	echo $word
done

for word in $(echo "To be, or not to be, that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take Arms against a Sea of troubles,
And by opposing end them: to die, to sleep
No more; and by a sleep, to say we end
The heart-ache, and the thousand natural shocks
That Flesh is heir to? 'Tis a consummation
Devoutly to be wished. To die, to sleep,
To sleep, perchance to Dream; aye, there's the rub,
For in that sleep of death, what dreams may come,
When we have shuffled off this mortal coil,
Must give us pause. There's the respect
That makes Calamity of so long life:
For who would bear the Whips and Scorns of time,
The Oppressor's wrong, the proud man's Contumely,
The pangs of dispised Love, the Law’s delay,
The insolence of Office, and the spurns
That patient merit of th'unworthy takes,
When he himself might his Quietus make
With a bare Bodkin? Who would Fardels bear, [F: these Fardels]
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscovered country, from whose bourn
No traveller returns, puzzles the will,
And makes us rather bear those ills we have,
Than fly to others that we know not of?
Thus conscience does make cowards of us all,
And thus the native hue of Resolution
Is sicklied o'er, with the pale cast of Thought,
And enterprises of great pitch and moment, [F: pith]
With this regard their Currents turn awry, [F: away]
And lose the name of Action. Soft you now,
The fair Ophelia? Nymph, in thy Orisons
Be all my sins remember'd. ")
do
	echo $word
done

for word in "$(echo "To be, or not to be, that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take Arms against a Sea of troubles,
And by opposing end them: to die, to sleep
No more; and by a sleep, to say we end
The heart-ache, and the thousand natural shocks
That Flesh is heir to? 'Tis a consummation
Devoutly to be wished. To die, to sleep,
To sleep, perchance to Dream; aye, there's the rub,
For in that sleep of death, what dreams may come,
When we have shuffled off this mortal coil,
Must give us pause. There's the respect
That makes Calamity of so long life:
For who would bear the Whips and Scorns of time,
The Oppressor's wrong, the proud man's Contumely,
The pangs of dispised Love, the Law’s delay,
The insolence of Office, and the spurns
That patient merit of th'unworthy takes,
When he himself might his Quietus make
With a bare Bodkin? Who would Fardels bear, [F: these Fardels]
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscovered country, from whose bourn
No traveller returns, puzzles the will,
And makes us rather bear those ills we have,
Than fly to others that we know not of?
Thus conscience does make cowards of us all,
And thus the native hue of Resolution
Is sicklied o'er, with the pale cast of Thought,
And enterprises of great pitch and moment, [F: pith]
With this regard their Currents turn awry, [F: away]
And lose the name of Action. Soft you now,
The fair Ophelia? Nymph, in thy Orisons
Be all my sins remember'd. ")"
do
	echo $word
done

for word in $CL4PTP
do
	echo $word
done | cat
echo $word

for word in $CL4PTP
do
	echo $word
done >&1 2>&1 <&- | tr a-z A-Z
echo $word

for i in 1 2 3 4
do
	cat
done <<TEST
"To be, or not to be, that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take Arms against a Sea of troubles,
And by opposing end them: to die, to sleep
No more; and by a sleep, to say we end
The heart-ache, and the thousand natural shocks
That Flesh is heir to? 'Tis a consummation
Devoutly to be wished. To die, to sleep,
To sleep, perchance to Dream; aye, there's the rub,
For in that sleep of death, what dreams may come,
When we have shuffled off this mortal coil,
Must give us pause. There's the respect
That makes Calamity of so long life:
For who would bear the Whips and Scorns of time,
The Oppressor's wrong, the proud man's Contumely,
The pangs of dispised Love, the Law’s delay,
The insolence of Office, and the spurns
That patient merit of th'unworthy takes,
When he himself might his Quietus make
With a bare Bodkin? Who would Fardels bear, [F: these Fardels]
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscovered country, from whose bourn
No traveller returns, puzzles the will,
And makes us rather bear those ills we have,
Than fly to others that we know not of?
Thus conscience does make cowards of us all,
And thus the native hue of Resolution
Is sicklied o'er, with the pale cast of Thought,
And enterprises of great pitch and moment, [F: pith]
With this regard their Currents turn awry, [F: away]
And lose the name of Action. Soft you now,
The fair Ophelia? Nymph, in thy Orisons
Be all my sins remember'd. "
TEST

for i in $(for i in $(for i in $CL4TP
do
echo $i;
done) 
do 
echo $i;
done)
do
echo $i;
done

for i in $(for i in $(for i in "Segmentation success"
do
echo $i;
./fdcheck | cat
done) 
do 
echo $i;
done)
do
echo $i;
done

for i in 1 2 3 4
do
./fdcheck
done

for word in $CL4PTP
do
	echo $word
	false
done >&1 2>&1 <&- | tr a-z A-Z > /dev/null
echo $?
echo $word

for word in 1 2 3 4
do
	false
done
echo $?

# TODO: more tests