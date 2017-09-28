/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alt_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/20 11:53:40 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 16:43:37 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void				ft_alt_beg_end(t_term *term, unsigned long key)
{
	t_leters		*tmp;

	tmp = term->hist.cur->hist_cmd;
	if (key == KEY_ALT_BEG)
	{
		while (tmp->cur->prev)
			ft_key_lft(term);
	}
	else if (key == KEY_ALT_END)
	{
		while (tmp->cur->next)
			ft_key_rght(term);
	}
}

void				ft_alt_up_dwn(t_term *term, unsigned long key)
{
	t_leters		*tmp;
	int				col;
	int				len;
	int				pozx;

	col = term->col;
	tmp = term->hist.cur->hist_cmd;
	if (key == KEY_ALT_UP && term->poz.y)
	{
		if (term->poz.x < (term->poz.x_prom + 1) && term->poz.y == 1)
			return ;
		while (col--)
			ft_key_lft(term);
	}
	else if (key == KEY_ALT_DWN)
	{
		len = term->hist.cur->hist_cmd->cnt + term->poz.x_prom;
		pozx = term->poz.x + term->poz.y * col;
		if ((len - pozx) < col)
			return ;
		while (col--)
			ft_key_rght(term);
	}
}

void				ft_alt_rght(t_term *term)
{
	t_leters		*tmp;
	int				state;
	int				nw;

	state = FALSE;
	nw = 0;
	tmp = term->hist.cur->hist_cmd;
	while (tmp->cur->next)
	{
		if (ft_isspace(tmp->cur->item))
			state = FALSE;
		else if (state == FALSE)
		{
			state = TRUE;
			if (nw++ == 1)
			{
				ft_key_lft(term);
				return ;
			}
		}
		ft_key_rght(term);
	}
}

void				ft_alt_lft(t_term *term)
{
	t_leters		*tmp;
	int				state;

	state = FALSE;
	tmp = term->hist.cur->hist_cmd;
	while (tmp->cur->prev)
	{
		if (ft_isspace(tmp->cur->item))
			state = FALSE;
		else if (state == FALSE)
		{
			state = TRUE;
			while (!ft_isspace(tmp->cur->item))
				ft_key_lft(term);
			return ;
		}
		ft_key_lft(term);
	}
}
