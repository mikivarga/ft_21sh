/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 16:27:13 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 15:03:32 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

void			ft_free(char **pp)
{
	int			i;

	i = 0;
	while (pp[i])
	{
		free(pp[i]);
		pp[i++] = NULL;
	}
	free(pp);
	pp = NULL;
}

void			ft_free_hist_str(t_term *t)
{
	t->hist.cur->hist_cmd->cur = t->hist.cur->hist_cmd->end;
	while (t->hist.cur->hist_cmd->cur)
	{
		ft_let_del(t);
		t->hist.cur->hist_cmd->cur = t->hist.cur->hist_cmd->cur->prev;
	}
}

void			ft_free_hist(t_term *t)
{
	t->hist.cur = t->hist.end;
	while (t->hist.cur)
	{
		ft_free_hist_str(t);
		t->hist.cur = t->hist.cur->prev;
	}
}
