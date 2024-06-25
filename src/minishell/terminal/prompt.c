/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 22:32:22 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 21:14:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Get the user name
static char	*get_user(t_data *data)
{
	char	*str;

	str = env_get(data->envp, "USER");
	if (!str || !*str)
		str = env_get(data->envp, "LOGNAME");
	if ((!str || !*str) && env_get(data->envp, "HOME"))
	{
		str = ft_strrchr(env_get(data->envp, "HOME"), '/');
		if (str)
			str += 1;
	}
	if (!str || !*str)
		str = "guest";
	return (ft_strjoin("\001"C"\002[\001"B_G"\002", str, 0));
}

//	Set the terminal prompt
void	set_prompt(t_data *data)
{
	char	*str;
	char	cwd[1024];

	str = get_user(data);
	str = ft_strjoin(str, "\001"C"\002]\001"G"\002-\001"Y"\002Mini", 1);
	str = ft_strjoin(str, "\001"BR"\002Shell\001"NC"\002:\001"B_B"\002", 1);
	if (getcwd(cwd, sizeof(cwd)))
		str = ft_strjoin(str, cwd, 1);
	else if (env_get(data->envp, "PWD"))
		str = ft_strjoin(str, env_get(data->envp, "PWD"), 1);
	else
		str = ft_strjoin(str, ".", 1);
	data->prompt = ft_strjoin(str, "\001"NC"\002$ \001"NC"\002", 1);
	str = env_get(data->envp, "HOME");
	if (!str || !*str || !ft_strstr(data->prompt, str))
		return ;
	data->extra = ft_strstr(data->prompt, str) - data->prompt;
	data->prompt = ft_replace(data->prompt, &data->extra, ft_strlen(str), "~");
	data->extra = 0;
}

//	Print text in the specified FD
int	print(t_data *data, int fd, char *s, int mode)
{
	int	result;

	result = 0;
	if ((mode == R || mode == RP || mode == FR || mode == FRP) && data->msg)
	{
		free(data->msg);
		data->msg = NULL;
	}
	if (s && !data->msg)
		data->msg = safe_strdup(data, s);
	else if (s && data->msg)
		data->msg = ft_strjoin(data->msg, s, 1);
	if ((mode == RP || mode == FRP || mode == P || mode == FP)
		&& data->msg && fd > 0)
	{
		result = write(fd, data->msg, ft_strlen(data->msg));
		free(data->msg);
		data->msg = NULL;
	}
	if (s && (mode == FR || mode == FRP || mode == FJ || mode == FP))
		free(s);
	return (result == -1);
}
