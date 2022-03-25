/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   global.c                                         -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/03/25 16:23:17 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/03/25 16:23:17 by csteenvo            :      ..            */
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
