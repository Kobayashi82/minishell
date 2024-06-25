/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 23:48:24 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 16:19:04 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	history_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, ft_strjoin("history version ", \
			(char *)rl_library_version, 0), FR);
		print(d, 1, " (part of readline)\n\n", J);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by Brian Fox, Free Software Foundation\n", P);
	}
	else
	{
		print(d, 1, "Usage: history [OPTION] \n\n", R);
		print(d, 1, "[no option]		display all commands\n", J);
		print(d, 1, " n			display the last n commands\n", J);
		print(d, 1, "-c			clears the command history\n", J);
		print(d, 1, "--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Get and set the spaces for the history index number
static char	*get_number(t_data *data, int len)
{
	char	*result;
	char	*number;
	int		calc_spaces;
	int		i;

	number = ft_itoa(history_length - len);
	if (!number)
		exit_error(data, NO_MEMORY, 1000, NULL);
	calc_spaces = ft_max(5 - ft_strlen(number), 0);
	result = safe_malloc(data, ft_max(6, ft_strlen(number) + 1));
	i = 0;
	while (calc_spaces--)
		result[i++] = ' ';
	result[i] = '\0';
	return (ft_strjoin(result, number, 3));
}

//	Prints history commands
static void	history_print(t_data *d, char *arg)
{
	int			len;
	HIST_ENTRY	*entry;

	len = -1;
	if (arg && *arg)
		len = ft_atoi(arg);
	if (len == -1 || len > history_length)
		len = history_length;
	if (len > 500)
		len = 500;
	print(d, 1, NULL, R);
	while (len--)
	{
		entry = history_get(history_length - len);
		if (entry && entry->line)
		{
			print(d, 1, ft_strjoin(get_number(d, len), "  ", 1), FJ);
			print(d, 1, ft_strjoin(entry->line, "\n", 0), FJ);
		}
	}
	print(d, 1, NULL, P);
}

//	Execute history built in
int	history(t_token *token, char **args)
{
	if (!ft_strcmp(args[1], "--help"))
		return (history_message(token->data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (history_message(token->data, 1));
	else if (!ft_strncmp(args[1], "-c", 2))
		return (rl_clear_history(), 0);
	else if (args[1] && args[2])
		return (exit_error(token->data, HIS_ARGS, 1, NULL));
	else if (args[1] && !ft_isdigit_s(args[1]))
		return (exit_error(token->data, HIS_DIG, 1, args[1]));
	history_print(token->data, args[1]);
	return (0);
}
