/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_sign.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adapassa <adapassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 18:04:10 by adapassa          #+#    #+#             */
/*   Updated: 2024/09/15 18:04:34 by adapassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// TODO: chiedere se e' un errore se quando sto scrivendo un comando e mando un ctrl+D non mi fa nulla 

void	sig_int(void)
{
	rl_on_new_line();
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_quit(void)
{
	rl_on_new_line();
	rl_redisplay();
	ft_putstr_fd("  \b\b", 1);
}

void	signal_handler(int signo)
{
	int	pid;
	int	status;

	pid = waitpid(-1, &status, 1);
	if (signo == SIGINT)
	{
		err_state = 130;
		if (pid == -1)
			sig_int();
		else
			ft_putstr_fd("\n", 1);
	}
	else if (signo == SIGQUIT)
	{
		if (pid == -1)
			sig_quit();
		else
			ft_putstr_fd("Quit: 3\n", 1);
	}
}

void	set_signal(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}
