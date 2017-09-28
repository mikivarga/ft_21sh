/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 18:06:06 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/03 18:08:35 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static void			ft_error_cd(char *dir)
{
	ft_putstr("cd: no such file or directory: ");
	ft_putendl(dir);
}

static char			*ft_check_way(char *arr, t_term *env)
{
	char		*dir;
	char		tmp;

	tmp = *arr;
	if (tmp == '~')
	{
		tmp = *(++arr);
		if (tmp == '/')
		{
			arr++;
			dir = ft_get_env("HOME", env);
			if (chdir(dir))
				ft_error_cd(dir);
			if (dir)
				free(dir);
		}
	}
	dir = ft_strdup(arr);
	return (dir);
}

static char			*ft_come_back_way(t_term *env)
{
	char		*pwd;
	char		*oldpwd;
	char		*dir;
	int			i;

	i = 0;
	pwd = ft_get_env("PWD", env);
	oldpwd = ft_get_env("OLDPWD", env);
	dir = ft_strdup(oldpwd + 1);
	ft_putchar('~');
	if (chdir("/"))
		ft_error_cd("/");
	while (pwd[i] == oldpwd[i])
		i++;
	if (pwd[i] == '\0')
		ft_putstr(&oldpwd[++i]);
	ft_putchar('\n');
	free(pwd);
	free(oldpwd);
	return (dir);
}

void				ft_cd(char **arr, t_term *env)
{
	char		*dir;

	dir = NULL;
	if (!*arr || !ft_strcmp(*arr, "~"))
		dir = ft_get_env("HOME", env);
	else if (!ft_strcmp(*arr, "-"))
		dir = ft_come_back_way(env);
	else
		dir = ft_check_way(*arr, env);
	if (chdir(dir))
		ft_error_cd(dir);
	else
	{
		if (dir)
			free(dir);
		dir = ft_get_env("PWD", env);
		ft_setcwd(dir, env);
	}
	if (dir)
		free(dir);
}
