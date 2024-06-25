/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 12:58:57 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/06 20:22:14 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	help(t_data *d)
{
	print(d, 1, "\n\tMINISHELL 1.0 (Builtins)\n\n", R);
	print(d, 1, " cd:\t\tChange the working directory\n", J);
	print(d, 1, " pwd:\t\tPrint the working directory\n", J);
	print(d, 1, " echo:\t\tPrint a message\n", J);
	print(d, 1, " env:\t\tList the environment and shell variables\n", J);
	print(d, 1, " export:\tSet environment variables\n", J);
	print(d, 1, " unset:\t\tDelete variables\n", J);
	print(d, 1, " exit:\t\tClose minishell\n", J);
	print(d, 1, " history:\tShow a list of previous commands\n", J);
	print(d, 1, " banner:\tDisplay the minishell banner\n", J);
	print(d, 1, " help:\t\tShow this help\n\n", J);
	print(d, 1, " All builtins comes with the flag --help and --version\n", J);
	print(d, 1, "\n Also, you can create shell variables using the format", J);
	print(d, 1, " 'VAR=VALUE'\n\n Check the minishell documentation for", J);
	print(d, 1, " a complete list of features\n\n", P);
	return (0);
}
