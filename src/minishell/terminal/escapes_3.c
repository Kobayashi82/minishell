/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escapes_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 18:46:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 16:18:35 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if a char is escape capable
char	escape_chars(char c)
{
	if (c == '\\')
		return ('\\');
	else if (c == 'n')
		return ('\n');
	else if (c == 'r')
		return ('\r');
	else if (c == 't')
		return ('\t');
	else if (c == 'f')
		return ('\f');
	else if (c == 'v')
		return ('\v');
	else if (c == 'a')
		return ('\a');
	else if (c == 'b')
		return ('\b');
	else if (c == '\'')
		return ('\'');
	else if (c == '"')
		return ('\"');
	else if (c == '?')
		return ('\?');
	return (c);
}

//	Check if a char in a string is escaped
bool	is_esc(char *str, int end)
{
	int		i;
	int		j;
	bool	esc;

	i = 0;
	esc = 0;
	while (str && str[i] && i < end)
	{
		if ((str[i] == '\'') && esc == false)
		{
			i++;
			j = i;
			while (str[i] && str[i] != '\'')
				i++;
			if (!str[i])
				i = j;
			continue ;
		}
		if (str[i] == '\\')
			esc = !esc;
		else if (esc == true)
			esc = false;
		i++;
	}
	return (esc);
}

void	trim_spaces(char *s1)
{
	int		i;

	i = 0;
	while (s1[i] && (i == 0 || !is_esc(s1, i)) && ft_isspace(s1[i]))
		i++;
	if (i > 0)
		ft_memcpy(s1, &s1[i], ft_strlen(&s1[i]) + 1);
	i = ft_strlen(s1) - 1;
	if (s1[i] == '\\' && (i == 0 || !is_esc(s1, i)))
		s1[i--] = '\0';
	while (i >= 0 && (i == 0 || !is_esc(s1, i)) && ft_isspace(s1[i]))
		s1[i--] = '\0';
}
