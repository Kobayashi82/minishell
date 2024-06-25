/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   about.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 23:43:46 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/10 18:28:26 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	about(t_data *d)
{
	print(d, 1, "\n\t\tMINISHELL 1.0\n\n", R);
	print(d, 1, "    by: vzurera-     &&     alvaquer\n\n", P);
	return (0);
}
