/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   global.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 12:00:29 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 12:00:29 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

const char	**pr_get_keywords(void)
{
	static const char	*keywords[] = {
		"if",
		"then",
		"else",
		"elif",
		"fi",
		"do",
		"done",
		"case",
		"esac",
		"while",
		"until",
		"for",
		"{",
		"}",
		"!",
		"in"
	};

	return (keywords);
}
