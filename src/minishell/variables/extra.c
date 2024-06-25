/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 20:45:18 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:07:28 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Join a shell variable
t_var	*join_shell_var(t_var **vars, char *name, char **content)
{
	t_var	*old_var;

	if (name && *name && name[ft_strlen(name) - 1] == '+')
	{
		name[ft_strlen(name) - 1] = '\0';
		old_var = shell_get(*vars, name, true);
		if (old_var)
		{
			*content = ft_strjoin(old_var->content, *content, 0);
			return (old_var);
		}
	}
	return (shell_get(*vars, name, true));
}

//	Join a environment variable
char	*join_env_var(char **envp, char *name, char *content)
{
	char	*old_var;
	char	*entry;

	if (name && *name && name[ft_strlen(name) - 1] == '+')
	{
		name[ft_strlen(name) - 1] = '\0';
		old_var = env_get(envp, name);
		if (old_var)
		{
			entry = ft_strjoin(old_var, content, 0);
			entry = ft_strjoin_c(name, entry, "=", 2);
			return (entry);
		}
	}
	return (ft_strjoin_c(name, content, "=", 0));
}
