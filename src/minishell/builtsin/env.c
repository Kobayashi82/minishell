/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 21:09:09 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/04 23:06:38 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	env_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "env version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P);
	}
	else
	{
		print(d, 1, "Usage: env [OPTION] \n\n", R);
		print(d, 1, "[no option]		display a list of all environment "
			"variables\n", J);
		print(d, 1, "-s			display a list of all shell variables\n", J);
		print(d, 1, "-0			end each output line with NULL, not ", J);
		print(d, 1, "newline\n--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Show an error if wrong option
static int	env_error(t_data *d, char *arg)
{
	if (arg[0] == '-' && arg[1] != '-')
	{
		while (*(++arg) && *arg == '0')
			;
		arg[1] = '\0';
		print(d, 2, ft_strjoin("env: invalid option -- '", arg, 0), FR);
		print(d, 2, "'\nTry 'env --help' for more information.\n", P);
	}
	else
	{
		print(d, 2, ft_strjoin("env: unrecognized option '", arg, 0), FR);
		print(d, 2, "'\nTry 'env --help' for more information.\n", P);
	}
	return (125);
}

//	Print environments variables or (-s) shell variables
static void	env_print(t_data *data, char **envp, char is_null, bool is_s)
{
	int		i;
	char	nl[2];
	t_var	*current;

	i = -1;
	print(data, 1, NULL, R);
	nl[0] = is_null * 10;
	nl[1] = '\0';
	if (!is_s)
		while (envp && envp[++i])
			print(data, 1, ft_strjoin(envp[i], nl, 0), FJ);
	else
	{
		current = data->vars;
		while (current)
		{
			if (current->name && !current->ex)
			{
				print(data, 1, ft_strjoin(current->name, "=", 0), FJ);
				print(data, 1, ft_strjoin(current->content, nl, 0), FJ);
			}
			current = current->next;
		}
	}
	print(data, 1, NULL, P);
}

//	Gets the options of the command
static bool	env_options(char *arg, bool *is_0, bool *is_s)
{
	int		i;
	bool	tmp_0;
	bool	tmp_s;

	i = 0;
	tmp_0 = *is_0;
	tmp_s = *is_s;
	if (arg && arg[i] == '-')
	{
		while (arg[++i])
		{
			if (arg[i] == '0')
				tmp_0 = true;
			else if (arg[i] == 's')
				tmp_s = true;
			else
				return (false);
		}
		*is_0 = tmp_0;
		*is_s = tmp_s;
		return (true);
	}
	return (false);
}

//	Execute env built in
int	env(t_data *data, char **args)
{
	bool	is_0;
	bool	is_s;
	bool	ended;
	int		i;

	is_0 = false;
	is_s = false;
	ended = false;
	i = 0;
	while (args && args[++i])
	{
		if (!ended && env_options(args[i], &is_0, &is_s))
			continue ;
		if (!ft_strcmp(args[i], "--help"))
			return (env_message(data, 0));
		else if (!ft_strcmp(args[i], "--version"))
			return (env_message(data, 1));
		else if (ft_strcmp(args[i], "--"))
			return (env_error(data, args[i]));
	}
	env_print(data, data->envp, !is_0, is_s);
	return (0);
}
