/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 23:19:38 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 11:18:44 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Merge two path lists to form a path list
static void	merge_lists(t_list **cwd_list, t_list *arg_list)
{
	t_list	*current;

	current = arg_list;
	while (current)
	{
		if (!strcmp((char *)current->content, ".."))
			ft_lstdel_last(cwd_list, free);
		else if (ft_strcmp((char *)current->content, "."))
			ft_lstadd_back(cwd_list, \
				ft_lstnew(strdup((char *)current->content)));
		current = current->next;
	}
}

//	Split a path into a path list
static t_list	*path_to_list(char *path)
{
	t_list	*path_l;
	char	*part;
	char	*start;
	char	*end;

	path_l = NULL;
	start = path;
	end = ft_memchr(start, '/', ft_strlen(start));
	while (end)
	{
		part = ft_substr(start, 0, end - start);
		ft_lstadd_back(&path_l, ft_lstnew(part));
		start = end + 1;
		end = ft_memchr(start, '/', ft_strlen(start));
	}
	if (*start)
		ft_lstadd_back(&path_l, ft_lstnew(ft_strdup(start)));
	return (path_l);
}

//	Join a path list into a path
static char	*list_to_path(t_data *data, t_list *cwd_list)
{
	char	*path;
	t_list	*current;

	path = safe_malloc(data, ft_lstsize(cwd_list) * 1024);
	memset(path, 0, ft_lstsize(cwd_list) * 1024);
	current = cwd_list;
	while (current)
	{
		if (ft_strlen(path) == 0 || (ft_strlen(path) > 0
				&& path[ft_strlen(path) - 1] != '/'))
			ft_strcat(path, "/");
		ft_strcat(path, (char *)current->content);
		current = current->next;
	}
	return (path);
}

//	Set the SHELL variable using PATH
static void	search_path(t_data *data, char *arg0)
{
	char	*path;

	path = create_path(data, arg0);
	shell_set(data, &data->vars, "MINISHELL", path);
	free(path);
}

//	Set the SHELL variable
void	shell_path(t_data *data, char *arg0)
{
	char	*path;
	char	cwd[1024];
	t_list	*cwd_list;
	t_list	*arg_list;

	if (*arg0 == '/' || *arg0 == '~')
		shell_set(data, &data->vars, "MINISHELL", arg0);
	else if (!ft_memchr(arg0, '/', ft_strlen(arg0)))
		search_path(data, arg0);
	else
	{
		getcwd(cwd, sizeof(cwd));
		cwd_list = path_to_list(cwd);
		arg_list = path_to_list(arg0);
		merge_lists(&cwd_list, arg_list);
		path = list_to_path(data, cwd_list);
		shell_set(data, &data->vars, "MINISHELL", path);
		ft_lstclear(&cwd_list, free);
		ft_lstclear(&arg_list, free);
		free(path);
	}
}
