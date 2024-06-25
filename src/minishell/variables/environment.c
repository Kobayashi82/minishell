/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 17:47:46 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/05 20:49:10 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Retrieves an environment variable
char	*env_get(char **envp, char *name)
{
	int	i;
	int	len;

	i = -1;
	if (!name)
		return (NULL);
	len = ft_strlen(name);
	while (envp && envp[++i])
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
	return (NULL);
}

//	Sets an environment variable
void	env_set(char ***envp, char *name, char *content)
{
	char	*entry;
	char	**env;
	int		i;

	i = 0;
	entry = join_env_var(*envp, name, content);
	if (!entry)
		return ;
	while (*envp && (*envp)[i])
	{
		if (ft_strncmp ((*envp)[i], name, ft_strlen(name)) == 0
			&& (*envp)[i][ft_strlen(name)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = entry;
			return ;
		}
		i++;
	}
	env = ft_realloc(*envp, (i + 1) * sizeof(char *), (i + 2) * sizeof(char *));
	if (!env)
		return (free(entry));
	env[i] = entry;
	env[i + 1] = NULL;
	*envp = env;
}

//	Deletes an environment variable
void	environment_unset(char ***envp, char *name)
{
	int	i;
	int	len;

	i = 0;
	if (!name)
		return ;
	len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, len) == 0 && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			while ((*envp)[i])
			{
				(*envp)[i] = (*envp)[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
}

//	Clears environment variables
void	*environment_clear(char ***envp)
{
	int	i;

	i = -1;
	while (envp && *envp && (*envp)[++i])
		free((*envp)[i]);
	if (envp && *envp)
	{
		free(*envp);
		*envp = NULL;
	}
	return (NULL);
}

//	Clones environment variables
char	**environment_clone(t_data *data, char **envp)
{
	char	**env;
	int		i;

	i = 0;
	while (envp && envp[i])
		i++;
	env = safe_malloc(data, (i + 1) * sizeof(char *));
	i = -1;
	while (envp[++i])
	{
		env[i] = ft_strdup(envp[i]);
		if (!env[i])
			return (environment_clear(&env));
	}
	env[i] = NULL;
	return (env);
}
