TEXT=$(curl https://gist.githubusercontent.com/ElliotGluck/64b0b814293c09999f765e265aaa2ba1/raw/79f24f9f87654d7ec7c2f6ba83e927852cdbf9a5/gistfile1.txt)

for word in $TEXT
do
	/bin/echo $word | cat | tr a-z A-Z
done