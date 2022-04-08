if true
then echo True
fi

if false
then echo False
fi

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

echo foo | if true
then tr 'a-z' 'A-Z'
fi

if true
then echo foo
fi | tr 'a-z' 'A-Z'
