int
	ft_iscntrl(int ch)
{
	return ((ch >= 000 && ch <= 037) || ch == 0177);
}
