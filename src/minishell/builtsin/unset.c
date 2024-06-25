/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:51:13 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:26:15 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	unset_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "unset version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P);
	}
	else
	{
		print(d, 1, "Usage: unset [NAME]...\n\n", R);
		print(d, 1, "--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Execute unset built in
int	unset(t_data *data, char **args)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (!ft_strcmp(args[1], "--help"))
		return (unset_message(data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (unset_message(data, 1));
	while (args && args[++i])
	{
		environment_unset(&data->envp, args[i]);
		shell_unset(&data->vars, args[i]);
		if (forbidden_chars(args[i], true))
		{
			status = 1;
			print(data, 2, ft_strjoin("minishell: unset: `", args[i], 0), FR);
			print(data, 2, "': not a valid identifier\n", P);
		}
	}
	return (status);
}
