/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 01:04:04 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/04 15:51:59 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	A malloc function with builtin exit_error
void	*safe_malloc(t_data *data, long bytes)
{
	void	*ptr;

	ptr = malloc(bytes);
	if (!ptr)
		exit_error(data, NO_MEMORY, 1001, NULL);
	return (ptr);
}

//	A calloc function with builtin exit_error
void	*safe_calloc(t_data *data, int count, long bytes)
{
	void	*ptr;

	ptr = ft_calloc(count, bytes);
	if (!ptr)
		exit_error(data, NO_MEMORY, 1001, NULL);
	return (ptr);
}

//	A strdup function with builtin exit_error
void	*safe_strdup(t_data *data, char *str)
{
	void	*ptr;

	ptr = ft_strdup(str);
	if (!ptr)
		exit_error(data, NO_MEMORY, 1001, NULL);
	return (ptr);
}

//	A dup2 function with builtin exit error
int	safe_dup2(t_data *data, int *fd1, int fd2, int close_fd)
{
	int	new_fd;

	new_fd = -1;
	if (*fd1 == -1)
		return (new_fd);
	if (*fd1 == fd2)
		return (0);
	new_fd = dup2(*fd1, fd2);
	if (close_fd)
		close(*fd1);
	if (new_fd == -1)
		exit_error(data, DUP_FAIL, 1, NULL);
	return (new_fd);
}
