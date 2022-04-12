FOO=1 | echo "foo=$FOO" | FOO=2
echo "foo=$FOO"
(sleep 0.1; echo a) | (sleep 0.2; echo b)
(sleep 0.2; echo a) | (sleep 0.1; echo b)