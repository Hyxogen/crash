int
	ft_isxdigit(int ch)
{
	return ((ch >= '0' && ch <= '9')
		|| (ch >= 'A' && ch <= 'F')
		|| (ch >= 'a' && ch <= 'f'));
}
