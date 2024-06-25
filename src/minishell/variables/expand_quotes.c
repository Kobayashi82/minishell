/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 00:15:28 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:20:24 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if the variable is a EXPORT_ONLY var
bool	export_only(t_data *data, char *name, char *content)
{
	if (name && content && !ft_strcmp(content, "[EXPORT_ONLY]"))
	{
		if (!env_get(data->envp, name))
			shell_set(data, &data->vars, name, content);
		return (true);
	}
	return (false);
}

//	Add single quotes arround every word in the variable expansion
static int	add_quote_2(char *new_s, char **s, int *i, int *j)
{
	if ((*i == 0 || !is_esc(*s, *i)) && ft_isspace((*s)[*i]))
	{
		if (ft_isspace_s(&(*s)[*i - 1]))
		{
			new_s[(*j)++] = '\'';
			new_s[(*j)++] = '\0';
			free(*s);
			*s = new_s;
			return (1);
		}
		if (new_s[*j - 1] != '\'')
			new_s[(*j)++] = '\'';
		while ((*s)[*i] && (*i == 0 || !is_esc(*s, *i))
			&& ft_isspace((*s)[*i]))
			(*i)++;
		if ((*s)[*i] && !is_esc(*s, *i - 1) && ft_isspace((*s)[*i - 1]))
		{
			new_s[(*j)++] = ' ';
			new_s[(*j)++] = '\'';
		}
	}
	if ((*s)[*i])
		new_s[(*j)++] = (*s)[(*i)++];
	return (0);
}

//	Add single quotes arround every word in the variable expansion
static char	*add_quote_1(char *new_s, char **s, int *i, int *j)
{
	while ((*s)[*i] && (*i == 0 || !is_esc(*s, *i)) && ft_isspace((*s)[*i]))
		(*i)++;
	while ((*s)[*i])
		if (add_quote_2(new_s, s, i, j))
			return (new_s);
	if (new_s[*j - 1] != '\'')
		new_s[(*j)++] = '\'';
	if (!is_esc(*s, *i - 1) && ft_isspace((*s)[*i - 1]))
		new_s[(*j)++] = ' ';
	new_s[*j] = '\0';
	free(*s);
	*s = new_s;
	return (new_s);
}

static bool	is_wildcards(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if ((s[i] == '"' || s[i] == '\'') && skip_quotes(s, &i))
			return (false);
		else if (ft_memchr("*?[", s[i], 3) && !is_esc(s, i))
			return (true);
		else
			i++;
	}
	return (false);
}

//	Add single quotes arround every word in the variable expansion
char	*add_quote(t_data *data, char **s)
{
	char	*new_s;
	int		i;
	int		j;

	i = 0;
	if (data->extra || is_wildcards(*s))
		return (*s);
	if (!*s || !*(*s))
		return (NULL);
	if (ft_isspace_s(*s))
	{
		free(*s);
		*s = ft_strdup(" ");
		return (*s);
	}
	j = ft_strlen(*s) * 2;
	new_s = malloc(j + 1);
	j = 0;
	if (!new_s)
		return (*s);
	if ((i == 0 || !is_esc(*s, i)) && ft_isspace((*s)[i]))
		new_s[j++] = ' ';
	new_s[j++] = '\'';
	return (add_quote_1(new_s, s, &i, &j));
}
