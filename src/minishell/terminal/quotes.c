/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 22:30:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 18:25:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Skip from the begining of the quote to the end
bool	skip_quotes(char *input, int *i)
{
	char	quote;
	int		orig;

	orig = *i;
	if (is_esc(input, *i))
	{
		(*i)++;
		return (0);
	}
	quote = input[*i];
	while (input[++(*i)] != quote
		|| (input[*i] == quote && input[*i] != '\'' && is_esc(input, *i)))
	{
		if (!input[*i])
		{
			*i = orig;
			return (0);
		}
	}
	return (0);
}

//	Skip from the begining of the parenthesis to the end
bool	skip_paren(char *input, int *i)
{
	int		pars;
	int		orig;

	orig = *i;
	if (is_esc(input, *i))
	{
		(*i)++;
		return (0);
	}
	pars = 1;
	if (input[*i] == '$')
		(++*i);
	while (input[(++*i)] && pars > 0)
	{
		if (input[*i] == '(')
			pars++;
		else if (input[*i] == ')')
			pars--;
	}
	*i -= 1;
	if (pars != 0)
		*i = orig + 1;
	return (0);
}
