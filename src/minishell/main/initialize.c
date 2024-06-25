/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 15:46:19 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 14:21:19 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Get PID of main process
static int	set_pid(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (0);
	else if (pid == 0)
		exit(0);
	waitpid(pid, NULL, 0);
	return (pid - 1);
}

//	Set the variable for Shell Level
static void	set_shlvl(t_data *data)
{
	char	*shlvl;
	int		shlvl_num;

	shlvl_num = ft_atoi(env_get(data->envp, "SHLVL")) + 1;
	if (shlvl_num < 0)
		shlvl_num = 0;
	shlvl = ft_itoa(shlvl_num);
	if (env_get(data->envp, "SHLVL")
		&& ft_atol(env_get(data->envp, "SHLVL")) && errno == ERANGE)
	{
		shlvl_num = 1;
		free(shlvl);
		shlvl = ft_itoa(shlvl_num);
	}
	else if (shlvl_num > 2000000)
	{
		exit_error(data, SHLVL_HIGH, 0, shlvl);
		shlvl_num = 1;
		free(shlvl);
		shlvl = ft_itoa(shlvl_num);
	}
	env_set(&data->envp, "SHLVL", shlvl);
	free(shlvl);
}

//	Initialize subshell data structure
void	initialize_subshell(t_data *data, t_data *sub_data, int fd, bool cl_fd)
{
	ft_memset(sub_data, 0, sizeof(t_data));
	sub_data->main = false;
	sub_data->is_subshell = true;
	sub_data->hd_n = data->hd_n;
	sub_data->exit_custom_code = 1000;
	sub_data->started = time(NULL);
	srand(sub_data->started);
	sub_data->bk_in = dup(STDIN_FILENO);
	sub_data->bk_out = dup(fd);
	sub_data->bk_err = dup(data->bk_err);
	if (sub_data->bk_in == -1 || sub_data->bk_out == -1
		|| sub_data->bk_err == -1)
		exit_error(data, DUP_FAIL, 1001, NULL);
	sub_data->envp = environment_clone(sub_data, data->envp);
	sub_data->vars = data->vars;
	set_shlvl(data);
	data->vars = NULL;
	if (cl_fd)
		close_fds(data->last_token, 0);
	close_fds(data->last_token, 1);
	data_free(data);
}

//	Initialize environment and shell variables
static void	initialize_variables(t_data *data, char **envp, char *arg0)
{
	char	cwd[1024];

	data->envp = environment_clone(data, envp);
	if (getcwd(cwd, sizeof(cwd)))
		env_set(&data->envp, "PWD", cwd);
	set_shlvl(data);
	if (!env_get(data->envp, "OLDPWD"))
		shell_set(data, &data->vars, "OLDPWD", "[EXPORT_ONLY]");
	if (!env_get(data->envp, "PATH"))
		shell_set(data, &data->vars, "PATH", \
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin");
	shell_path(data, arg0);
	env_set(&data->envp, "_", env_get(data->envp, "SHELL"));
	shell_set(data, &data->vars, "MINISHELL_VERSION", "1.0-release");
}

//	Initialize data structure
void	initialize(t_data *data, char **envp, char *arg0)
{
	g_signal = 0;
	ft_memset(data, 0, sizeof(t_data));
	data->main = true;
	tcgetattr(STDIN_FILENO, &data->term);
	data->hd_n = -1;
	data->exit_custom_code = 1000;
	data->started = time(NULL);
	srand(data->started);
	data->pid = set_pid();
	data->bk_in = dup(STDIN_FILENO);
	data->bk_out = dup(STDOUT_FILENO);
	data->bk_err = dup(STDERR_FILENO);
	if (data->bk_in == -1 || data->bk_out == -1 || data->bk_err == -1)
		exit_error(data, DUP_FAIL, 1001, NULL);
	initialize_variables(data, envp, arg0);
}
