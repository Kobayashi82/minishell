/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 19:15:44 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/29 20:40:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Close and free some data from expand_input()
int	expand_closer(int fd, char *text)
{
	if (fd != -1)
		close(fd);
	if (text)
		free(text);
	return (0);
}

//	Open, expand a redirection input and write the result
int	expand_input(t_data *data, t_redir *redir)
{
	int			fd;
	int			readed;
	char		*text;
	struct stat	st;

	if (!redir->expand)
		return (0);
	text = NULL;
	fd = open(redir->file, O_RDONLY);
	if (fstat(fd, &st) == -1)
		return (expand_closer(fd, text));
	text = malloc(st.st_size + 1);
	if (text == NULL)
		return (expand_closer(fd, text));
	readed = read(fd, text, st.st_size);
	if (readed == -1)
		return (expand_closer(fd, text));
	text[readed] = '\0';
	expand_heredoc(data, &text);
	close(fd);
	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd != -1 && text)
		write(fd, text, ft_strlen(text));
	return (expand_closer(fd, text));
}
