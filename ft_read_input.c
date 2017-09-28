/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 19:13:43 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:18:58 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static void		ft_add_l(t_term *term, unsigned char key)
{
	term->poz.x += 1;
	if (term->poz.x == term->col && key != '\n')
		tputs(tgetstr("do", NULL), 1, ft_put);
}

void			ft_add_let(t_term *term, unsigned char key)
{
	if (term->poz.x == term->col)
	{
		tputs(tgetstr("up", NULL), 1, ft_put);
		tputs(tgoto(tgetstr("ch", NULL), 0, term->poz.x), 1, ft_put);
		term->poz.x = 0;
		term->poz.y += 1;
	}
	else if (key == '\n')
	{
		term->poz.x = 0;
		term->poz.y += 1;
	}
	if (!term->hist.cur->hist_cmd->cur->next)
	{
		ft_add_letters_end(term->hist.cur->hist_cmd, key);
		term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->end;
	}
	else
	{
		ft_add_letters_middle(term->hist.cur->hist_cmd, key);
		term->hist.cur->hist_cmd->cur = term->hist.cur->hist_cmd->cur->next;
		ft_show_lst_str(term->hist.cur->hist_cmd, term);
	}
	ft_add_l(term, key);
}

static void		ft_navig_keys(t_term *term)
{
	unsigned long		key;

	key = 0;
	if (read(term->fd_tty, &key, sizeof(key)))
	{
		if (key == KEY_UP || key == KEY_DWN)
			ft_key_up_dw(term, key);
		else if (key == KEY_LFT)
			ft_key_lft(term);
		else if (key == KEY_RGHT)
			ft_key_rght(term);
		else if (key == KEY_DEL)
			ft_key_del(term);
		else if (key == KEY_ALT_LFT)
			ft_alt_lft(term);
		else if (key == KEY_ALT_RGHT)
			ft_alt_rght(term);
		else if (key == KEY_ALT_UP || key == KEY_ALT_DWN)
			ft_alt_up_dwn(term, key);
		else if (key == KEY_ALT_BEG || key == KEY_ALT_END)
			ft_alt_beg_end(term, key);
	}
}

int				ft_read_input(t_term *term)
{
	unsigned char		key;
	ssize_t				nr;

	while ((nr = (read(0, &key, sizeof(unsigned char)))) > 0)
	{
		ft_windsize(term);
		if (key == '\n')
			return (ft_check(term));
		if (key == ESC)
			ft_navig_keys(term);
		else if (key == KEY_BACKSP)
			ft_key_del(term);
		else if (ft_isascii(key) && key)
			ft_add_let(term, key);
		key = 0;
	}
	if (nr == -1)
		return (WARNING);
	return (TRUE);
}
