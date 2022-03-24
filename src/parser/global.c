/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   global.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmeijer <dmeijer@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 11:30:24 by dmeijer       #+#    #+#                 */
/*   Updated: 2022/03/24 11:31:40 by dmeijer       ########   odam.nl         */
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
