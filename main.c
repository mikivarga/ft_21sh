/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/25 14:22:35 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/13 15:03:10 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

int				ft_put(int c)
{
	int			fd;

	fd = open("/dev/tty", O_WRONLY);
	write(fd, &c, 1);
	close(fd);
	return (TRUE);
}

void			ft_save_buf(t_term *t, char *buf)
{
	t->hist.cur->hist_cmd->cur = t->hist.cur->hist_cmd->beg->next;
	while (t->hist.cur->hist_cmd->cur)
	{
		*buf++ = t->hist.cur->hist_cmd->cur->item;
		t->hist.cur->hist_cmd->cur = t->hist.cur->hist_cmd->cur->next;
	}
	*buf = '\0';
}

static int		ft_read_cmd(t_term *cmd, char *buf)
{
	int				ret;
	static int		flag = 0;

	ft_open_visual_mode(cmd);
	ft_show_prom(cmd);
	cmd->hist.cur = cmd->hist.end;
	if (!flag++ || cmd->hist.end->hist_cmd->cnt != 1)
	{
		ft_add_history(&(cmd->hist));
		cmd->hist.cur = cmd->hist.end;
		ft_add_letters_end(cmd->hist.cur->hist_cmd, ' ');
		cmd->hist.cur->hist_cmd->cur = cmd->hist.cur->hist_cmd->end;
	}
	else
	{
		cmd->hist.cur = cmd->hist.end;
		write(1, " ", 1);
		cmd->hist.cur->hist_cmd->cur = cmd->hist.cur->hist_cmd->end;
	}
	ret = ft_read_input(cmd);
	ft_save_buf(cmd, buf);
	ft_close_visual_mode(cmd);
	return (ret);
}

void			ft_setcwd(char *dir, t_term *cmd)
{
	extern char		**environ;
	char			cwd[BUF];

	if (!getcwd(cwd, BUF))
		exit(EXIT_FAILURE);
	if (!dir)
		ft_new_env(cmd, environ);
	else
		ft_setenv("OLDPWD", dir, cmd);
	ft_setenv("PWD", cwd, cmd);
}

int				main(void)
{
	t_term		term;
	char		buf[BUF];
	int			flag;

	ft_setcwd(NULL, &term);
	ft_init_history(&term);
	ft_setenv("PROM", "$>", &term);
	while (TRUE)
	{
		flag = ft_read_cmd(&term, buf);
		ft_let_del(&term);
		if (flag == -1)
			exit(EXIT_FAILURE);
		else if (!flag)
			continue ;
		if (!ft_parser(&term, buf))
			break ;
	}
	ft_free(term.env);
	ft_free_hist(&term);
	return (0);
}
