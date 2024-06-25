/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escapes_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 01:00:17 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 19:43:58 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if a char is special
static bool	special_char(char c, bool quoted)
{
	if (c == '"')
		return (true);
	if (c == '\\')
		return (true);
	if (c == '$')
		return (true);
	if (c == 't' && !quoted)
		return (true);
	return (false);
}

//	Trim an arg of spaces and double quotes
static void	trim_esc_3(char *arg, t_trim *t)
{
	t->token->quoted = true;
	ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
	t->j++;
	while (arg && arg[t->i] && !(arg[t->i] == '"' && !is_esc(t->copy, t->j)))
	{
		if (arg[t->i] == '\\'
			&& (t->convert || special_char(arg[t->i + 1], t->quoted)))
		{
			ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
			t->j++;
			if (t->convert)
				arg[t->i] = escape_chars(arg[t->i]);
		}
		t->i++;
		t->j++;
	}
	if (arg[t->i] == '"')
	{
		ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
		t->j++;
	}
}

//	Trim an arg of spaces and single quotes
static void	trim_esc_2(char *arg, t_trim *t)
{
	t->token->quoted = true;
	ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
	t->j++;
	while (arg && arg[t->i] && !(arg[t->i] == '\'' && !is_esc(t->copy, t->j)))
	{
		t->i++;
		t->j++;
	}
	if (arg[t->i] == '\'')
	{
		ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
		t->j++;
	}
}

//	Trim an arg of spaces and quotes
static void	trim_esc_1(char *arg, t_trim *t)
{
	while (arg && arg[t->i])
	{
		if (arg[t->i] == '\'' && !is_esc(t->copy, t->j))
			trim_esc_2(arg, t);
		else if (arg[t->i] == '"' && !is_esc(t->copy, t->j))
			trim_esc_3(arg, t);
		else if (arg[t->i] == '\\')
		{
			ft_memmove(&arg[t->i], &arg[t->i + 1], ft_strlen(&arg[t->i]));
			if (special_char(arg[t->i], 0))
			{
				t->i++;
				t->j++;
			}
			t->j++;
		}
		else if (arg[t->i])
		{
			t->i++;
			t->j++;
		}
	}
}

//	Trim an arg of spaces and quotes
int	trim_esc(t_token *token, char *arg, bool convert, bool quoted)
{
	t_trim	t;

	t.i = 0;
	t.j = 0;
	t.copy = ft_strdup(arg);
	t.convert = convert;
	t.quoted = quoted;
	t.token = token;
	trim_esc_1(arg, &t);
	free(t.copy);
	return (0);
}
