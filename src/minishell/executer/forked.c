/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forked.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 19:18:25 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:44:47 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Clone the information to pass them to execve
static void	clone_exec(t_token *token)
{
	char	*cmd;
	char	**args;
	char	**envp;

	cmd = token->cmd;
	args = token->args;
	envp = token->data->envp;
	token->cmd = NULL;
	token->args = NULL;
	token->data->envp = NULL;
	data_free(token->data);
	execve(cmd, args, envp);
	free_array(args);
	free_array(envp);
	if (access(cmd, R_OK) == -1)
	{
		cmd = ft_strjoin("minishell: ", cmd, 2);
		cmd = ft_strjoin(cmd, ": Permission denied\n", 1);
		write(2, cmd, ft_strlen(cmd));
		free(cmd);
		exit(126);
	}
	free(cmd);
	exit(0);
}

//	Validate a command (exists, is a directory, permission...)
static int	valid_cmd(t_token *token, char *path)
{
	if (ft_memchr(token->cmd, '/', ft_strlen(token->cmd))
		&& access(token->cmd, F_OK) == -1)
	{
		close_fds(token, 0);
		exit_error(token->data, OPEN_NO_FILE, 1127, token->cmd);
	}
	else if (!ft_memchr(token->cmd, '/', ft_strlen(token->cmd)))
	{
		close_fds(token, 0);
		if (!path || !*path)
			exit_error(token->data, OPEN_NO_FILE, 1127, token->cmd);
		else
			exit_error(token->data, CMD_NO_FILE, 1127, token->cmd);
	}
	if (is_directory(token->cmd))
	{
		close_fds(token, 0);
		exit_error(token->data, CMD_DIR, 1126, token->cmd);
	}
	if (access(token->cmd, X_OK) == -1)
	{
		close_fds(token, 0);
		exit_error(token->data, CMD_EXEC, 1126, token->cmd);
	}
	return (0);
}

//	Set last status code and signal before forking
static void	set_forked_signals(t_token *token)
{
	if (token->data->main)
		signal(SIGQUIT, sigquit_handler);
	if (token->data->main && !token->data->reset_status)
	{
		token->data->reset_status = true;
		token->data->last_status = 0;
	}
}

//	Fork main process and execute the token on the child process
int	forking(t_token *token)
{
	set_forked_signals(token);
	token->pid = fork();
	if (token->pid == 0)
	{
		token->data->last_status = 0;
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		token->data->main = false;
		token->data->is_subshell = false;
		expand(token, false);
		redirects(token, true);
		build_path(token->data, token);
		if (!(token->redirection || !token->cmd || is_builtin(token->args[0])))
			valid_cmd(token, env_get(token->data->envp, "PATH"));
		set_redirections(token, true);
		if (!token->cmd || token->redirection || builtsin(token))
			exit_error(token->data, NOTHING, 1000, NULL);
		clone_exec(token);
	}
	close_fds(token, 0);
	if (token->pid < 0)
		exit_error(token->data, FORK_FAIL, 1001, NULL);
	return (0);
}
