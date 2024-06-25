/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 22:35:06 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 16:13:01 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Check if is a valid digit
static bool	is_digit_num(char *arg)
{
	int		i;

	i = 0;
	if (arg && arg[i])
	{
		while (ft_isspace(arg[i]))
			i++;
		if (arg[i] == '-' || arg[i] == '+')
			i++;
		while (arg[i])
		{
			if (!ft_isdigit(arg[i]) && !(arg[i] != 13 && ft_isspace(arg[i])))
				return (false);
			i++;
		}
		ft_atol(arg);
		if (errno == ERANGE)
			return (false);
	}
	return (true);
}

//	Close the main program
int	exit_minishell(t_token *token, char **args)
{
	t_data	*data;
	char	*arg;

	data = token->data;
	data->exit = true;
	if (token->data->main && !token->data->as_arg)
		print(token->data, 1, "exit\n", RP);
	if (args && args[0] && args[1])
	{
		arg = escape_input(args[1]);
		if (!is_digit_num(arg))
			data->exit_custom_code = exit_error(data, EXIT_NUM, 255, args[1]);
		else if (args[2])
			data->exit_custom_code = exit_error(data, EXIT_ARGS, 1, NULL);
		else
			data->exit_custom_code = ft_atol(arg);
		free(arg);
	}
	if (data->exit_custom_code != 1000)
		data->last_status = data->exit_custom_code;
	return (data->last_status);
}
