/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvarga <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 17:53:29 by mvarga            #+#    #+#             */
/*   Updated: 2017/08/13 14:20:07 by mvarga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_21sh.h"

static char			*ft_waycat(char *buf, char *name, char *path)
{
	char			*str;

	str = buf;
	while (*path && *path != ':')
		*str++ = *path++;
	if (str != buf)
		*str++ = '/';
	while (*name)
		*str++ = *name++;
	*str = '\0';
	return (*path ? ++path : NULL);
}

static int			ft_cmd_exec(char *program, char **arr, t_term *env)
{
	char			*path;
	char			*tmp;
	char			fullpath[BUFSIZ];

	path = ft_get_env("PATH", env);
	tmp = ft_strchr(program, '/') ? "" : path;
	while (tmp)
	{
		tmp = ft_waycat(fullpath, program, tmp);
		if (!access(fullpath, X_OK))
		{
			execve(fullpath, arr, env->env);
			free(path);
			return (TRUE);
		}
	}
	free(path);
	return (FALSE);
}

static int			ft_child(char *name, char **arr, t_term *env)
{
	if (env->fin)
	{
		if (!ft_input(env))
		{
			ft_putendl("21sh: err inp");
			return (-1);
		}
	}
	if (env->fout)
	{
		if (!ft_output(env))
		{
			ft_putendl("21sh: err out");
			return (-1);
		}
	}
	if (!ft_strcmp(name, "echo"))
		ft_echo(arr + 1);
	else if (!ft_cmd_exec(name, arr, env))
	{
		ft_putstr("21sh: command not found: ");
		ft_putendl(name);
	}
	exit(1);
}

static int			ft_new_comand(char *name, char **arr, t_term *env)
{
	pid_t			pid;
	int				status;

	pid = fork();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
		ft_child(name, arr, env);
	if (waitpid(pid, &status, 0) == -1)
		return (-1);
	else if (WIFEXITED(status))
		return (WIFEXITED(status));
	else if (WIFSIGNALED(status))
		return (WIFSIGNALED(status));
	else if (WIFSTOPPED(status))
		return (WIFSTOPPED(status));
	return (-1);
}

int					ft_select_cmd(char **arr, t_term *env, int flag)
{
	if (!ft_strcmp(*arr, "cd"))
		ft_cd(arr + 1, env);
	else if (!ft_strcmp(*arr, "env"))
		ft_show_env(env->env);
	else if (!ft_strcmp(*arr, "setenv"))
		ft_setenv(*(arr + 1), *(arr + 2), env);
	else if (!ft_strcmp(*arr, "unsetenv"))
		ft_unsetenv(*(arr + 1), env);
	else if (!ft_strcmp(*arr, "exit"))
	{
		ft_putendl("exit");
		return (FALSE);
	}
	else
	{
		if (flag)
			ft_new_comand(*arr, arr, env);
		else
			ft_cmd_exec(*arr, arr, env);
	}
	return (TRUE);
}
