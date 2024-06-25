/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 21:13:25 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/07 23:56:03 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print help or version message
static int	cd_message(t_data *d, int type)
{
	if (type)
	{
		print(d, 1, "cd version 1.0 (part of minishell)\n\n", R);
		print(d, 1, "This is free software: ", J);
		print(d, 1, "you are free to change and redistribute it.\n", J);
		print(d, 1, "There is NO WARRANTY, to the extent permitted by law.", J);
		print(d, 1, "\n\nWritten by vzurera- (Kobayashi82)\n", P);
	}
	else
	{
		print(d, 1, "Usage: cd [PATH]\n\n", R);
		print(d, 1, "~			change to home directory\n", J);
		print(d, 1, "-			change to previous directory\n", J);
		print(d, 1, "--version		output version information\n", J);
		print(d, 1, "--help			display this help\n\n", P);
	}
	return (0);
}

//	Change directory to de previous path
static int	is_dash(t_data *data, char **args)
{
	char	*tmp;
	char	*pwd;
	char	*oldpwd;
	char	cwd[1024];

	if (!ft_strcmp(args[1], "-"))
	{
		pwd = env_get(data->envp, "PWD");
		if ((!pwd || !*pwd) && getcwd(cwd, sizeof(cwd)))
			tmp = safe_strdup(data, cwd);
		else
			tmp = safe_strdup(data, pwd);
		oldpwd = env_get(data->envp, "OLDPWD");
		if (!oldpwd || !*oldpwd)
			return (free(tmp), exit_error(data, CD_OLDPWD, 1, NULL));
		if (chdir(oldpwd))
			return (free(tmp), exit_error(data, CD_ERROR, 1, \
				env_get(data->envp, "OLDPWD")));
		print(data, 1, ft_strjoin(oldpwd, "\n", 0), FRP);
		env_set(&data->envp, "PWD", oldpwd);
		env_set(&data->envp, "OLDPWD", tmp);
		free(tmp);
		return (1);
	}
	return (0);
}

//	Check if HOME path is not set
static void	check_home(t_token *token, char *path)
{
	if (path && *path == '~' && (token->args_lst
			&& token->args_lst->next && token->args_lst->next->arg
			&& !(token->args_lst->next->arg[0] == '"'
				|| token->args_lst->next->arg[0] == '\'')))
	{
		if (!env_get(token->data->envp, "HOME"))
			exit_error(token->data, CD_HOME, 1, NULL);
	}
}

//	Change to the indicated directory
static int	change_dir(t_token *token, char *path)
{
	char	cwd[1024];

	check_home(token, path);
	if (path && *path && chdir(path))
	{
		if (errno == EACCES)
			return (exit_error(token->data, CD_PER, 1, path));
		else
			return (exit_error(token->data, CD_PATH, 1, path));
	}
	env_set(&token->data->envp, "OLDPWD", \
		env_get(token->data->envp, "PWD"));
	if (getcwd(cwd, sizeof(cwd)))
		env_set(&token->data->envp, "PWD", cwd);
	return (0);
}

//	Execute cd built in
int	cd(t_token *token, char **args)
{
	char	*home;

	if (!args[1] || (!ft_strcmp(args[1], "--") && !args[2]))
	{
		home = env_get(token->data->envp, "HOME");
		if (!home || !*home)
			return (exit_error(token->data, CD_HOME, 1, NULL));
		return (change_dir(token, home));
	}
	else if (!ft_strcmp(args[1], "--") && args[2] && !args[3])
		return (change_dir(token, args[2]));
	else if (!ft_strcmp(args[1], "--help"))
		return (cd_message(token->data, 0));
	else if (!ft_strcmp(args[1], "--version"))
		return (cd_message(token->data, 1));
	else if (is_dash(token->data, args))
		return (0);
	return (change_dir(token, args[1]));
}
