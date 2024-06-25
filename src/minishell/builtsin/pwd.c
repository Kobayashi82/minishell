/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 21:14:39 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/04 23:01:39 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	pwd_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "pwd version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P);
	}
	else
	{
		print(d, 1, "Usage: pwd\n\n", R);
		print(d, 1, "--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Execute pwd built in
int	pwd(t_token *token, char **args)
{
	char	*pwd;
	char	cwd[1024];

	if (!ft_strcmp(args[1], "--help"))
		return (pwd_message(token->data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (pwd_message(token->data, 1));
	if (getcwd(cwd, sizeof(cwd)))
		print(token->data, 1, ft_strjoin(cwd, "\n", 0), FRP);
	else
	{
		pwd = env_get(token->data->envp, "PWD");
		if (pwd)
			print(token->data, 1, ft_strjoin(pwd, "\n", 0), FRP);
	}
	return (0);
}
