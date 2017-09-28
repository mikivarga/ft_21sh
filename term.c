/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:45:26 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 18:49:08 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

t_term				*g_t;

void				ft_term_init(t_term *term)
{
	if ((tgetent(NULL, getenv("TERM"))) > 0)
	{
		term->fd_tty = open("/dev/tty", O_RDWR | O_NDELAY);
		ioctl(term->fd_tty, TIOCGETA, &(term->old));
		term->new = term->old;
		term->col = tgetnum("co");
		term->lin = tgetnum("li");
		term->new.c_lflag &= ~(ICANON | ECHO);
		term->new.c_cc[VMIN] = 1;
		term->new.c_cc[VTIME] = 0;
	}
	else
		ft_putendl("No TERM detected!");
}

static void			ft_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		ft_show_prom(g_t);
		g_t->hist.cur = g_t->hist.end;
		if (g_t->hist.end->hist_cmd->cnt != 1)
		{
			ft_add_history(&(g_t->hist));
			g_t->hist.cur = g_t->hist.end;
			ft_add_letters_end(g_t->hist.cur->hist_cmd, ' ');
		}
		else
			write(1, " ", 1);
		g_t->hist.cur->hist_cmd->cur = g_t->hist.cur->hist_cmd->end;
		return ;
	}
	else
		exit(sig);
}

void				ft_show_env(char **env)
{
	while (*env)
		ft_putendl(*env++);
}

void				ft_open_visual_mode(t_term *term)
{
	ft_term_init(term);
	ioctl(term->fd_tty, TIOCSETA, &(term->new));
	g_t = term;
	if (signal(SIGWINCH, NULL) == SIG_ERR)
		exit(EXIT_FAILURE);
	if (signal(SIGINT, ft_signal_handler) == SIG_ERR)
		exit(EXIT_FAILURE);
}

void				ft_close_visual_mode(t_term *term)
{
	ioctl(term->fd_tty, TIOCSETA, &(term->old));
}
