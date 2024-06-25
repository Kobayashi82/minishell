/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herestring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 15:59:59 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/03 19:55:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Replace a string inside a string with another string XD
char	*ft_replace(char *str, int *start, int len, char *replace)
{
	int		new_len;
	char	*new_str;
	int		i;

	if (!start)
		i = 0;
	else
		i = *start;
	new_len = ft_strlen(str) - len + ft_strlen(replace);
	new_str = malloc(new_len + 1);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	ft_memcpy(new_str, str, i);
	ft_memcpy(new_str + i, replace, ft_strlen(replace));
	ft_memcpy(new_str + i + ft_strlen(replace), \
		str + i + len, ft_strlen(str + i + len));
	new_str[new_len] = '\0';
	i += ft_strlen(replace);
	if (start)
		*start = i;
	free(str);
	return (new_str);
}

//	Generate a new filename for a temp file
char	*generate_tmp_name(t_data *data)
{
	char	*new_file;

	new_file = NULL;
	while (++data->hd_n < 100)
	{
		free(new_file);
		new_file = safe_strdup(data, "/tmp/hd_");
		new_file = ft_strjoin(new_file, ft_itoa(data->hd_n), 3);
		if (!new_file)
			exit_error(data, NO_MEMORY, 1001, NULL);
		if (access(new_file, F_OK) == -1)
			break ;
	}
	if (data->hd_n == 100)
	{
		free(new_file);
		return (safe_strdup(data, ""));
	}
	return (new_file);
}

//	Expand and replace all variables in a string
static void	expand_herestring(t_data *data, char **str)
{
	int		i;
	int		e;

	i = 0;
	pre_subshell_var(data, str);
	while ((*str) && (*str)[i])
	{
		if ((*str)[i] == '\'' && skip_quotes(*str, &i))
			return ;
		else if ((*str)[i] == '$' && !is_esc(*str, i) && (*str)[i + 1] == '(')
			i++;
		else if ((*str)[i] == '$' && !is_esc(*str, i)
			&& ft_memchr("$?_0", (*str)[i + 1], 4))
			exval(data, str, &i, ft_substr(&(*str)[i + 1], 0, 1));
		else if ((*str)[i] == '$' && !is_esc(*str, i))
		{
			e = i + 1;
			while ((*str)[e] && (ft_isalnum((*str)[e]) || (*str)[e] == '_'))
				e++;
			exval(data, str, &i, ft_substr(&(*str)[i + 1], 0, (e - i) - 1));
		}
		else
			i++;
	}
	trim_arg(data->last_token, *str);
}

//	Create a herestring
int	herestring(t_data *data, t_redir **redir, bool forked)
{
	char	*text;
	int		fd;

	if (!data->main && !data->is_subshell && (*redir)->type == DLT)
		exit_error(data, SUB_HEREDOC, 1000, (*redir)->file);
	if ((*redir)->type == TLT)
	{
		(*redir)->type = LT;
		text = ft_strjoin((*redir)->file, "\n", 1);
		expand_herestring(data, &text);
		text = ft_strjoin(text, "\n", 1);
		(*redir)->file = generate_tmp_name(data);
		fd = open((*redir)->file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd != -1 && text)
			write(fd, text, ft_strlen(text));
		else if (fd == -1)
		{
			exit_error(data, OPEN_FAIL, 1, NULL);
			if (forked)
				fd = expand_closer(-1, text) + exit_error(data, 0, 1000, NULL);
			return (expand_closer(-1, text) + 1);
		}
		expand_closer(fd, text);
	}
	return (0);
}
