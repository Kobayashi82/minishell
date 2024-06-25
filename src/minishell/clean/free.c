/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 19:58:41 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/29 19:42:57 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_tmp(t_data *data)
{
	t_args	*current;
	char	*path;

	path = "/tmp/hd_*";
	if (data->main)
	{
		wildcards(data, &path);
		current = data->files;
		while (current)
		{
			unlink(current->arg);
			current = current->next;
		}
	}
}

//	Free the data structure
void	data_free(t_data *data)
{
	clear_tmp(data);
	environment_clear(&data->envp);
	shell_clear(&data->vars);
	args_list_clear(&data->files);
	tokens_clear(data);
	if (data->input)
		free(data->input);
	if (data->msg)
		free(data->msg);
	close(data->bk_in);
	close(data->bk_out);
	close(data->bk_err);
}

//	Free a char array
void	free_array(char **array)
{
	char	**original_array;

	original_array = array;
	while (array && *array)
		free(*(array++));
	if (original_array)
		free(original_array);
}
