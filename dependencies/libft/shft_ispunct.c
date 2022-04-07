int
	ft_ispunct(int ch)
{
	return ((ch >= '!' && ch <= '/')
		|| (ch >= ':' && ch <= '@')
		|| (ch >= '[' && ch <= '`')
		|| (ch >= '{' && ch <= '~'));
}
