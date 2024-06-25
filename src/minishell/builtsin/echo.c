/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:02:26 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 18:08:23 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	echo_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "echo version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		return (print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P));
	}
	print(d, 1, "Usage: echo [OPTION] [ARGS]...\n\n", R);
	print(d, 1, "-n			do not output the trailing newline\n", J);
	print(d, 1, "-e			enable interpretation of backslash escapes\n", J);
	print(d, 1, "--version		output version information\n", J);
	return (print(d, 1, "--help			display this help\n\n", P));
}

//	Prints the string passed as an argument
static void	echo_print(t_data *data, char *arg, bool more)
{
	print(data, 1, arg, J);
	if (more)
		print(data, 1, " ", J);
}

//	Gets the options of the command
static bool	echo_options(char *arg, bool *is_n, bool *is_e)
{
	int		i;
	bool	tmp_n;
	bool	tmp_e;

	i = 0;
	tmp_e = *is_e;
	tmp_n = *is_n;
	if (arg && arg[i] == '-')
	{
		if (!arg[i + 1])
			return (false);
		while (arg[++i])
		{
			if (arg[i] == 'n')
				tmp_n = true;
			else if (arg[i] == 'e')
				tmp_e = true;
			else
				return (false);
		}
		*is_n = tmp_n;
		*is_e = tmp_e;
		return (true);
	}
	return (false);
}

static char	*get_arg(t_token *token, int i, bool is_e)
{
	t_args	*current;
	char	*tmp;

	current = token->args_lst;
	while (current && i--)
		current = current->next;
	if (current)
	{
		trim_spaces(current->arg);
		tmp = current->arg;
		current->arg = escape_input(current->arg);
		free(tmp);
		trim_esc(token, current->arg, is_e, false);
		return (current->arg);
	}
	return (NULL);
}

//	Execute echo built in
int	echo(t_token *token, char **args)
{
	bool	is_n;
	bool	is_e;
	bool	ended;
	int		i;

	is_n = false;
	is_e = false;
	ended = false;
	i = 0;
	print(token->data, 1, NULL, R);
	if (!ft_strcmp(args[1], "--help"))
		return (echo_message(token->data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (echo_message(token->data, 1));
	while (args && args[++i])
	{
		if (!ended && echo_options(args[i], &is_n, &is_e))
			continue ;
		ended = true;
		echo_print(token->data, get_arg(token, i, is_e), args[i + 1]);
	}
	if (!is_n)
		return (print(token->data, 1, "\n", P));
	return (print(token->data, 1, NULL, P));
}
