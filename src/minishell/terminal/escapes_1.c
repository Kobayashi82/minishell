/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escapes_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 18:06:18 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 16:25:53 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Converts a char to octal
static int	escape_oct(char *str, char *processed, int *i, int *j)
{
	char	oct[4];
	int		k;

	oct[0] = '\0';
	oct[1] = '\0';
	oct[2] = '\0';
	oct[3] = '\0';
	k = -1;
	if (ft_isdigit(str[*i + 1]) && ft_isdigit(str[*i + 2])
		&& ft_isdigit(str[*i + 3]))
	{
		while (++k < 3)
			oct[k] = str[*i + 1 + k];
		processed[(*j)++] = (char)ft_atol_base(oct, NULL, 8);
		*i += 1 + k;
		return (1);
	}
	return (0);
}

//	Converts a char to hexadecimal
static int	escape_hex(char *str, char *processed, int *i, int *j)
{
	char	hex[3];
	int		k;

	hex[0] = '\0';
	hex[1] = '\0';
	hex[2] = '\0';
	k = -1;
	if (str[*i + 1] == 'x' && ft_isxdigit(str[*i + 2]))
	{
		while (++k < 2 && ft_isxdigit(str[*i + 2 + k]))
			hex[k] = str[*i + 2 + k];
		processed[(*j)++] = (char)ft_atol_base(hex, NULL, 16);
		*i += 2 + k;
		return (1);
	}
	return (0);
}

//	Replace escape sequences with their corresponding characters
char	*escape_input(char *str)
{
	char	*processed;
	int		i;
	int		j;

	i = 0;
	j = 0;
	processed = malloc((ft_strlen(str) + 1) * sizeof(char));
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			if (escape_hex(str, processed, &i, &j))
				continue ;
			else if (escape_oct(str, processed, &i, &j))
				continue ;
		}
		processed[j++] = str[i++];
	}
	processed[j] = '\0';
	return (processed);
}
