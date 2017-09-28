/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 20:28:03 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:50:02 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void				ft_poz_x_y(t_term *term)
{
	term->poz.x = 1;
	term->poz.y += 1;
}

void				ft_key_l(t_term *term)
{
	tputs(tgetstr("le", NULL), 1, ft_put);
	term->poz.x -= 1;
}

void				ft_key_r(t_term *term)
{
	if (term->poz.x == term->col)
		ft_poz_x_y(term);
	else
		term->poz.x += 1;
	tputs(tgetstr("nd", NULL), 1, ft_put);
}

void				ft_windsize(t_term *term)
{
	t_win		ws;
	int			len;

	ioctl(term->fd_tty, TIOCGWINSZ, &ws);
	if (term->col != ws.ws_col)
	{
		len = term->poz.y * term->col + term->poz.x;
		term->poz.y = len / ws.ws_col;
		term->poz.x = len % ws.ws_col;
		term->col = ws.ws_col;
		term->lin = ws.ws_row;
	}
}

void				ft_check_hist(t_term *t, t_cursor *poz)
{
	(void)poz;
	if (t->hist.cur->hist_cmd->cnt != 1)
	{
		ft_add_history(&(t->hist));
		tputs(tgetstr("cd", NULL), 1, ft_put);
		tputs(tgetstr("le", NULL), 1, ft_put);
		ft_add_letters_end(t->hist.end->hist_cmd, ' ');
		t->hist.end->hist_cmd->cur = t->hist.end->hist_cmd->end;
	}
	else
		tputs(tgetstr("cd", NULL), 1, ft_put);
}
