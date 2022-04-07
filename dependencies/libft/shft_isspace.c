int
	ft_isspace(int ch)
{
	return (ch == '\t' || ch == '\n' || ch == '\v'
		|| ch == '\f' || ch == '\r'|| ch == ' ');
}
