/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsetenv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/11 17:02:34 by mvarga            #+#    #+#             */
/*   Updated: 2017/07/11 17:04:26 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static int		ft_check_value(char *name, t_term *cmd)
{
	char		**pp;

	if (!name)
	{
		ft_putendl("usage: unsetenv [env]");
		return (FALSE);
	}
	pp = cmd->env;
	while (*pp)
	{
		if (ft_envname_cmp(name, *pp))
			return (TRUE);
		pp++;
	}
	ft_putendl("incorect [env]");
	return (FALSE);
}

static int		ft_cnt_env(char **env)
{
	int			cnt;

	cnt = -1;
	while (*env++)
		cnt++;
	return (cnt);
}

void			ft_unsetenv(char *name, t_term *cmd)
{
	char		**newenv;
	int			cnt;
	int			i;
	int			j;

	i = 0;
	if (!ft_check_value(name, cmd))
		return ;
	cnt = ft_cnt_env(cmd->env);
	if ((newenv = (char **)malloc(sizeof(char *) * (cnt + 1))))
	{
		while (i < cnt + 1)
			newenv[i++] = NULL;
		i = 0;
		j = 0;
		while (cmd->env[i])
		{
			if (!ft_envname_cmp(name, cmd->env[i]))
				newenv[j++] = ft_strdup(cmd->env[i]);
			free(cmd->env[i++]);
		}
		free(cmd->env);
		cmd->env = newenv;
	}
}
