/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   banner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 14:13:30 by vzurera-          #+#    #+#             */
/*   Updated: 2024/05/10 18:29:13 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//	Print the banner when launching MiniShell
void	print_banner(t_data *d)
{
	print(d, 1, BR"\n\t————————", R);
	print(d, 1, "——————————————————"Y, J);
	print(d, 1, "——————————————", J);
	print(d, 1, "——————————————————\n", J);
	print(d, 1, BR"\t| "Y" __ __   _   __  _   _   "BR, J);
	print(d, 1, " __   _  _   ___   _     _    "Y"|\n", J);
	print(d, 1, BR"\t| "Y"|  V  | | | |  \\| | | | "BR, J);
	print(d, 1, "/' _/ | || | | __| | |   | |   "Y"|\n", J);
	print(d, 1, BR"\t| "Y"| \\_/ | | | | | ' | | | "BR, J);
	print(d, 1, "`._`. | >< | | _|  | |_  | |_  "Y"|\n", J);
	print(d, 1, BR"\t| "Y"|_| |_| |_| |_|\\__| |_| "BR, J);
	print(d, 1, "|___/ |_||_| |___| |___| |___| "Y"|\n", J);
	print(d, 1, BR"\t|                                    ", J);
	print(d, 1, "                    "Y"|\n", J);
	print(d, 1, BR"\t|  "C"As beautiful as a shell   ", J);
	print(d, 1, Y" alvaquer"C"   &"BR"   vzurera-    "Y"|\n"NC, J);
	print(d, 1, BR"\t———————————————", J);
	print(d, 1, "———————————"Y, J);
	print(d, 1, "———————————————————", J);
	print(d, 1, "—————————————", J);
	print(d, 1, "\n\n"NC, P);
}

void	print_welcome(t_data *d)
{
	print(d, 1, "\n\t\t\tWELCOME TO MINISHELL 1,0\n\n", R);
	print(d, 1, "\t This project has been made with effort and care\n", J);
	print(d, 1, "\t Feel free to try anything you want\n", J);
	print(d, 1, "\t To get information about the builtins you can type", J);
	print(d, 1, " 'help'\n\n    Check the minishell documentation for", J);
	print(d, 1, " a complete list of features\n\n", P);
}

void	first_start(t_data *data)
{
	if (access(".first_start", F_OK) != -1)
	{
		print_banner(data);
		print_welcome(data);
		unlink(".first_start");
	}
}
