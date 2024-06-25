/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 18:19:36 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 10:59:18 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if an arg is a variable assignation
bool	is_var(char *arg)
{
	char	*name;
	char	*pos;
	bool	valid;

	name = NULL;
	valid = false;
	pos = ft_memchr(arg, '=', ft_strlen(arg));
	if (pos && (pos - arg) > 0)
	{
		name = ft_substr(arg, 0, (pos - arg));
		if (forbidden_chars(name, false))
			return (false);
		valid = true;
		free(name);
	}
	return (valid);
}

//	Check if all args in an args list are variable assignments
bool	is_vars(t_token *token)
{
	t_args	*args;

	args = token->args_lst;
	while (args)
	{
		if (is_builtin(args->arg))
			return (true);
		if (!is_var(args->arg))
			return (false);
		args = args->next;
	}
	return (true);
}

//	Get the name of the variable removing the last '+' if exist
static int	real_name(t_data *data, char *name, char **rname)
{
	*rname = NULL;
	if (!name || !*name)
		return (0);
	*rname = safe_strdup(data, name);
	if (*rname && (*rname)[ft_strlen(*rname) - 1] == '+')
		(*rname)[ft_strlen(*rname) - 1] = '\0';
	return (0);
}

//	Set a environment variable
static void	set_var_env(t_data *data, char *name, char *content, char *rnme)
{
	if (!content && shell_get(data->vars, rnme, true))
		env_set(&data->envp, name, shell_get(data->vars, rnme, 1)->content);
	else
	{
		if (!env_get(data->envp, rnme))
		{
			shell_set(data, &data->vars, name, content);
			env_set(&data->envp, name, \
				shell_get(data->vars, rnme, 1)->content);
		}
		else
			env_set(&data->envp, name, content);
	}
	shell_unset(&data->vars, rnme);
}

//	Set or remove variables depending of the parameters
void	set_var(t_data *data, char *name, char *content, bool is_env)
{
	char	*rnme;

	if (!real_name(data, name, &rnme) && export_only(data, rnme, content))
	{
		free(rnme);
		return ;
	}
	else if (rnme && is_env)
		set_var_env(data, name, content, rnme);
	else if (rnme)
	{
		if (env_get(data->envp, rnme))
			env_set(&data->envp, name, content);
		else if (shell_get(data->vars, rnme, 1)
			&& shell_get(data->vars, rnme, 1)->ex)
		{
			env_set(&data->envp, name, content);
			shell_unset(&data->vars, rnme);
		}
		else
			shell_set(data, &data->vars, name, content);
	}
	free(rnme);
}
