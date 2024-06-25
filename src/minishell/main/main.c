/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 17:07:31 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/11 01:46:00 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	NOT IMPLEMENTED:	|&, <>, 2&>
//	FAILING:			"'", '"', $_ in subshell
//	Entry point of the program
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	initialize(&data, envp, argv[0]);
	if (argc == 2 && !ft_strncmp(argv[1], "-c", 2))
		exit_error(&data, START_ARGS, 1002, NULL);
	else if (argc > 1 && ft_strncmp(argv[1], "-c", 2))
		argument_error(&data, argv[1]);
	else if (argc > 2 && !ft_strncmp(argv[1], "-c", 2))
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		data.as_arg = true;
		data.input = safe_strdup(&data, argv[2]);
		if (ft_strlen(data.input) > 1
			&& !is_esc(data.input, ft_strlen(data.input) - 2)
			&& !ft_strcmp(&data.input[ft_strlen(data.input) - 2], "\\\n"))
			data.input[ft_strlen(data.input) - 2] = '\0';
		process_input(&data);
	}
	else
		while (argc && !data.exit && !read_input(&data))
			;
	if (data.main && !data.exit && !data.as_arg)
		print(&data, 1, "exit\n", RP);
	exit_error(&data, NOTHING, 1000, NULL);
}
