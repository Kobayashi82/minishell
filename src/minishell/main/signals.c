/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/01 19:11:19 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/08 16:38:45 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

//	Handle SIGQUIT signal
void	sigquit_handler(int sig)
{
	(void) sig;
	write(1, "Quit\n", 5);
}

//	Handle SIGINT signal
void	sigint_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = 130;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

//	Handle SIGINT signal
void	sigint_killer(int sig)
{
	if (sig == SIGINT)
		g_signal = 130;
}
