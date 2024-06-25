/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 21:06:09 by vzurera-          #+#    #+#             */
/*   Updated: 2024/04/30 13:47:42 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Process the tokens in the subshell command
static void	subshell_child(t_data *data, t_data *sub_data, int *fd, char *arg)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGINT, sigint_killer);
	close(fd[0]);
	data->main = false;
	initialize_subshell(data, sub_data, fd[1], true);
	close(fd[1]);
	sub_data->input = arg;
	process_input(sub_data);
	data_free(sub_data);
	if (sub_data->extra == -1)
		exit(234 % 256);
	else
		exit(sub_data->last_status % 256);
}

//	Wait for the subshell to finish and get its exit status
static char	*wait_child(t_data *data, int pid, int fd)
{
	int			status;

	waitpid(pid, &status, 0);
	if (!data->exit || data->exit_custom_code == 1000)
	{
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = 128 + WTERMSIG(status);
	}
	signal(SIGQUIT, SIG_IGN);
	data->last_status = status;
	if (status == 131)
		return (safe_strdup(data, ""));
	return (get_output(data, fd));
}

//	Create a pipe and a fork to execute and redirect the output of the subshell
static char	*subshell(t_data *data, char *arg)
{
	t_data		sub_data;
	int			pid;
	int			fd[2];

	if (data->main)
		signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sigint_killer);
	if (pipe(fd) == -1)
	{
		free(arg);
		exit_error(data, PIPE_FAIL, 1, NULL);
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
		subshell_child(data, &sub_data, fd, arg);
	free(arg);
	close(fd[1]);
	if (pid < 0)
	{
		close(fd[0]);
		exit_error(data, FORK_FAIL, 1001, NULL);
	}
	return (wait_child(data, pid, fd[0]));
}

//	Call the subshell execution and replace the subshell command with output
static void	execute_subshell(t_data *data, char **arg, int *start, int *end)
{
	char	*n_arg;
	char	*output;

	n_arg = ft_substr(*arg, *start, (*end + 1) - *start);
	ft_memmove(n_arg, n_arg + 2, ft_strlen(n_arg + 1));
	ft_memmove(n_arg + ft_strlen(n_arg) - 1, n_arg + ft_strlen(n_arg), 2);
	output = subshell(data, n_arg);
	*arg = ft_replace(*arg, start, (*end + 1) - *start, output);
	free(output);
}

//	Execute subshells and delete/replace parenthesis with result
void	pre_subshell_var(t_data *data, char **arg)
{
	int		i;
	int		start;

	i = 0;
	data->extra = 0;
	while (*arg && (*arg)[i])
	{
		if ((*arg)[i] == '"' && !is_esc(*arg, i))
			data->extra = !data->extra;
		if (!data->extra && (*arg)[i] == '\'' && skip_quotes(*arg, &i))
			return ;
		else if ((*arg)[i] == '(' && skip_paren(*arg, &i))
			return ;
		else if (((*arg)[i] == '$' && !is_esc(*arg, i)) && (*arg)[i + 1] == '(')
		{
			start = i;
			if (!skip_paren(*arg, &i) && start == i - 1)
				continue ;
			execute_subshell(data, arg, &start, &i);
			i = start;
		}
		else
			i++;
	}
}
