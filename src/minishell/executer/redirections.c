/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/07 18:40:02 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 12:03:55 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Reset redirections
void	reset_redirections(t_data *data, bool in, bool out, bool err)
{
	if (in)
		safe_dup2(data, &data->bk_in, STDIN_FILENO, 0);
	if (out)
		safe_dup2(data, &data->bk_out, STDOUT_FILENO, 0);
	if (err)
		safe_dup2(data, &data->bk_err, STDERR_FILENO, 0);
}

//	Get the output of a subshell command
char	*get_output(t_data *data, int fd)
{
	char	*line;

	print(data, 1, NULL, R);
	while (true)
	{
		line = ft_get_next_line(fd);
		if (!line)
			break ;
		print(data, 1, line, FJ);
	}
	if (data->msg == NULL)
	{
		close(fd);
		return (safe_strdup(data, ""));
	}
	close(fd);
	line = data->msg;
	if (line && line[0] && line[ft_strlen(line) - 1] == '\n')
		line[ft_strlen(line) - 1] = '\0';
	data->msg = NULL;
	return (line);
}

//	Open input files and redirects STDIN
static int	input(t_data *data, t_token *tk, t_redir *redir, bool forked)
{
	int	fd;

	fd = -1;
	if (herestring(data, &redir, forked))
		return (1);
	trim_arg(tk, redir->file);
	if (redir->ambiguous)
		data->last_status = exit_error(data, OPEN_AMB, 1, redir->file);
	else if (access(redir->file, F_OK) == -1)
		data->last_status = exit_error(data, OPEN_NO_FILE, 1, redir->file);
	else if (access(redir->file, R_OK) == -1)
		data->last_status = exit_error(data, OPEN_READ, 1, redir->file);
	else
	{
		expand_input(data, redir);
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
			data->last_status = exit_error(data, OPEN_FAIL, 1, redir->file);
	}
	if (fd == -1 && forked)
		fd = close_fds(tk, 0) + exit_error(data, NOTHING, 1000, NULL);
	else if (fd == -1 && !forked)
		return (1);
	safe_dup2(data, &fd, STDIN_FILENO, 1);
	return (0);
}

//	Open output files and redirects STDOUT
static int	output(t_data *data, t_token *tk, t_redir *redir, bool forked)
{
	int	fd;

	fd = -1;
	trim_arg(tk, redir->file);
	if (redir->ambiguous)
		data->last_status = exit_error(data, OPEN_AMB, 1, redir->file);
	else if (is_directory(redir->file))
		data->last_status = exit_error(data, OPEN_DIR, 1, redir->file);
	else
	{
		if (redir->type == GT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
		else if (redir->type == DGT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0664);
		if (fd == -1 && errno == EACCES)
			data->last_status = exit_error(data, OPEN_READ, 1, redir->file);
		else if (fd == -1)
			data->last_status = exit_error(data, OPEN_FAIL, 1, redir->file);
	}
	if (fd == -1 && forked)
		fd = close_fds(tk, 0) + exit_error(data, NOTHING, 1000, NULL);
	else if (fd == -1 && !forked)
		return (1);
	safe_dup2(data, &fd, STDOUT_FILENO, 1);
	return (0);
}

//	Set input, output and pipes redirections
int	set_redirections(t_token *tk, bool f)
{
	t_redir	*c;

	safe_dup2(tk->data, &tk->data->bk_in, STDIN_FILENO, 0);
	safe_dup2(tk->data, &tk->data->bk_out, STDOUT_FILENO, 0);
	close_fds(tk, 1);
	safe_dup2(tk->data, &tk->fd[0], STDIN_FILENO, 1);
	safe_dup2(tk->data, &tk->fd[1], STDOUT_FILENO, 1);
	c = tk->redir;
	while (c)
	{
		if (!tk->data->main && !tk->data->is_subshell && c->type == DLT)
		{
			print(tk->data, 2, "minishell: warning: here-document ", R);
			print(tk->data, 2, "delimited by end-of-file (wanted `", J);
			print(tk->data, 2, ft_strjoin(c->file, "')\n", 0), FP);
			tk->data->extra = open("/dev/null", O_RDONLY);
			safe_dup2(tk->data, &tk->data->extra, STDIN_FILENO, 1);
			tk->data->extra = 0;
		}
		if (((c->type == 1 || c->type == 4) && input(tk->data, tk, c, f))
			|| ((c->type == 2 || c->type == 5) && output(tk->data, tk, c, f)))
			return (1);
		c = c->next;
	}
	return (0);
}
