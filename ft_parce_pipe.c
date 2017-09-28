/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parce_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 17:17:54 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/12 17:13:43 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static void			ft_midle_child(int i, int cnt, t_term *env)
{
	int			j;

	if (i == cnt)
	{
		j = env->fd[1] - 2;
		while (j > 2)
			close(j--);
		close(env->fd[1]);
		close(0);
		dup(env->fd[0]);
		close(env->fd[0]);
		return ;
	}
	close(env->fd[1] - 2);
	j = env->fd[1] - 4;
	while (j > 2)
		close(j--);
	close(0);
	dup(env->fd[0] - 2);
	close(env->fd[0] - 2);
	close(env->fd[0]);
	close(1);
	dup(env->fd[1]);
	close(env->fd[1]);
}

static void			ft_child(int i, int cnt, char **arr, t_term *env)
{
	if (env->fout && !((env->rout & AGRE) && i == cnt))
		ft_output(env);
	if (env->fin && !((env->rout & AGRE) && i == cnt))
		ft_input(env);
	if (i == 0)
	{
		close(env->fd[0]);
		close(1);
		dup(env->fd[1]);
		close(env->fd[1]);
	}
	else if (i == 1 && cnt == i)
	{
		close(env->fd[1]);
		close(0);
		dup(env->fd[0]);
		close(env->fd[0]);
	}
	else
		ft_midle_child(i, cnt, env);
	ft_select_cmd(arr, env, FALSE);
	exit(1);
}

static char			**ft_retsore_arr(int i, int cnt, char **arr)
{
	if (i != cnt)
	{
		while (*arr)
			arr++;
		*arr = ft_strdup("|");
		arr++;
	}
	return (arr);
}

static int			ft_exe_pipe(int cnt, char **arr, t_term *env)
{
	pid_t			pid;
	int				status;
	int				i;

	i = 0;
	while (i <= cnt)
	{
		if (i != cnt)
			pipe(env->fd);
		if ((pid = fork()) < 0)
		{
			ft_putendl("Fork Error");
			exit(1);
		}
		else if (pid == 0)
			ft_child(i, cnt, arr, env);
		arr = ft_retsore_arr(i++, cnt, arr);
	}
	i = env->fd[1];
	while (i > 2)
		close(i--);
	while (wait(&status) > 0)
		;
	return (TRUE);
}

int					ft_parse_pipe(t_term *env, char **arr)
{
	int				cnt_pipe;
	char			**pp;

	cnt_pipe = 0;
	pp = arr;
	while (*pp)
	{
		if (*(*pp) == '|')
		{
			free(*pp);
			*pp = NULL;
			cnt_pipe++;
		}
		pp++;
	}
	if (cnt_pipe)
		return (ft_exe_pipe(cnt_pipe, arr, env));
	return (WARNING);
}
