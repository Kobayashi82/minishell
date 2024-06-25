/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildpath.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 11:03:04 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 09:23:20 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Convert an args list to an array (to pass them to execve or builtins)
static void	create_array(t_token *token)
{
	t_args	*current;
	char	**array;
	int		size;
	int		i;

	size = 0;
	current = token->args_lst;
	while (current)
	{
		size++;
		current = current->next;
	}
	array = safe_malloc(token->data, (size + 1) * sizeof(char *));
	current = token->args_lst;
	i = 0;
	while (current)
	{
		array[i] = ft_strdup(current->arg);
		current = current->next;
		i++;
	}
	array[size] = NULL;
	token->args = array;
}

//	Join a path and a filename
static char	*join_path(char *path, char *arg0)
{
	char	cwd[1024];

	if (!*path && getcwd(cwd, sizeof(cwd)))
		return (ft_strjoin_c(cwd, arg0, "/", 0));
	if (path && path[ft_strlen(path) - 1] != '/')
		return (ft_strjoin_c(path, arg0, "/", 0));
	return (ft_strdup(""));
}

//	Create the program path using PATH variable
char	*create_path(t_data *data, char *arg0)
{
	char	*path_var;
	char	**paths;
	char	*fullpath;
	int		i;

	i = -1;
	if (!arg0 || !*arg0 || !ft_strncmp(arg0, "./", 2)
		|| ft_memchr(arg0, '/', ft_strlen(arg0)))
		return (ft_strdup(arg0));
	path_var = env_get(data->envp, "PATH");
	if (!path_var && shell_get(data->vars, "PATH", false))
		path_var = shell_get(data->vars, "PATH", false)->content;
	if (!path_var)
		return (ft_strdup(arg0));
	paths = ft_split(path_var, ':');
	while (paths && paths[++i])
	{
		fullpath = join_path(paths[i], arg0);
		if (access(fullpath, F_OK) != -1)
			return (free_array(paths), fullpath);
		free(fullpath);
	}
	free_array(paths);
	return (ft_strdup(arg0));
}

//	Trim an arg of spaces and quotes
int	trim_arg(t_token *token, char *arg)
{
	trim_spaces(arg);
	trim_esc(token, arg, false, true);
	return (0);
}

//	Get the fullpath of the program  (remove_quotes(token->args[i]);)
int	build_path(t_data *data, t_token *token)
{
	int	i;

	i = -1;
	if (token->type)
		return (0);
	create_array(token);
	while (token->args && token->args[++i])
		trim_arg(token, token->args[i]);
	free(token->cmd);
	if (token->args[0] && !is_builtin(token->args[0]))
	{
		i = -1;
		while (token->args[0][++i])
			if (token->args[0][i] != '/' && token->args[0][i] != '.')
				break ;
		if (token->args[0][i])
			token->cmd = create_path(data, token->args[0]);
		else
			token->cmd = safe_strdup(data, token->args[0]);
	}
	else
		token->cmd = safe_strdup(data, token->args[0]);
	return (0);
}
