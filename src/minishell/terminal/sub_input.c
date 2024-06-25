/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 23:36:45 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 14:10:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Remove comments from the input
void	clean_comments(char *line)
{
	int		i;

	i = 0;
	while (line && line[i])
	{
		if ((line[i] == '"' || line[i] == '\'') && skip_quotes(line, &i))
			;
		else if (line[i] == '#' && (i == 0 || ft_isspace(line[i - 1]))
			&& !is_esc(line, i))
		{
			line[i] = '\0';
			break ;
		}
		else
			i++;
	}
}

//	Manage quotation marks to support commands with multi-lines
int	open_quote(t_data *data, char *str, bool reset)
{
	bool	escaped;

	escaped = false;
	if (reset || g_signal == 130)
	{
		data->quoted = 0;
		return (0);
	}
	while (str && *str)
	{
		if (*str == '\\' && data->quoted != '\'')
			escaped = str++;
		if ((*str == '"' || *str == '\'') && escaped == false)
		{
			if (data->quoted == *str)
				data->quoted = 0;
			else if (!data->quoted)
				data->quoted = *str;
		}
		escaped = false;
		if (*str)
			str++;
	}
	return (data->quoted);
}

//	Manage parenthesis to support commands with multi-lines
int	open_pars(char *input, bool reset)
{
	static int	pars;
	int			in_quotes;
	int			i;

	i = 0;
	in_quotes = 0;
	if (reset || g_signal == 130)
		pars = 0;
	while (!reset && g_signal == 0 && input && input[i])
	{
		if ((input[i] == '"' || input[i] == '\'') && !is_esc(input, i))
		{
			if (!in_quotes)
				in_quotes = input[i];
			else if (in_quotes == input[i])
				in_quotes = 0;
		}
		if (!in_quotes && input[i] == '(' && !is_esc(input, i))
			pars++;
		if (!in_quotes && input[i] == ')' && !is_esc(input, i))
			pars--;
		i++;
	}
	return (pars);
}

//	Check if the input continue on the next line
bool	open_line(t_data *data, char *input)
{
	int	last;

	if (!input || g_signal == 130)
		return (false);
	if (open_quote(data, input, false))
		return (true);
	if (open_pars(input, false))
		return (true);
	last = ft_strlen(input) - 1;
	while (last && ft_isspace(input[last]) && !is_esc(input, last))
		last--;
	if (input[last] == '\\' && !is_esc(input, last))
	{
		data->quoted = '\\';
		return (true);
	}
	else if (!ft_strncmp(&input[last - 1], "||", 2) && !is_esc(input, last - 1))
		return (true);
	else if (!ft_strncmp(&input[last - 1], "&&", 2) && !is_esc(input, last - 1))
		return (true);
	else if (input[last] == '|' && !is_esc(input, last))
		return (true);
	return (false);
}

//	Read the sub lines in the input
void	read_sub_input(t_data *data, char **input)
{
	free(*input);
	*input = readline(NC"> ");
	if (data->quoted == '\\')
	{
		data->quoted = 0;
		data->input[ft_strlen(data->input) - 1] = '\0';
		data->input = ft_strjoin(data->input, *input, 1);
	}
	else if (data->quoted != 0)
		data->input = ft_strjoin_c(data->input, *input, "\n", 1);
	else
	{
		if (ft_isspace(data->input[ft_strlen(data->input) - 1])
			&& !is_esc(data->input, ft_strlen(data->input) - 1))
			data->input = ft_strjoin(data->input, *input, 1);
		else
			data->input = ft_strjoin_c(data->input, *input, " ", 1);
	}
	clean_comments(data->input);
}
