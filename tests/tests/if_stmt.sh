if true
then echo Yes
else echo No
fi

if false
then echo Yes
else echo No
fi

if false
then echo Yes
elif true
then echo No
else echo Maybe
fi

if false
then echo Yes
elif false
then echo No
else echo Maybe
fi