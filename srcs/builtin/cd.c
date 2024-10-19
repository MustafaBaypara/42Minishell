/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:27:16 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 17:45:48 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static char	*home_path(t_env *home, t_command *cmd)
{
	char	*path;

	if (home)
	{
		path = check_malloc(ft_strdup(home->value));
		return (path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", cmd->fd[1]);
		_global(NULL)->error_no = 1;
		return (check_malloc(ft_calloc(1, 1)));
	}
}

static char	*old_pwd(t_command *cmd, t_env *oldpwd)
{
	char	*path;

	if (oldpwd)
	{
		path = check_malloc(ft_strdup(oldpwd->value));
		ft_putendl_fd(path, cmd->fd[1]);
		return (path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", cmd->fd[1]);
		_global(NULL)->error_no = 1;
		return (check_malloc(ft_calloc(1, 1)));
	}
}

static char	*get_path(t_command *cmd, t_env *oldpwd, t_env *home)
{
	if (cmd->value[1])
	{
		if (cmd->value[1][0] != '-')
			return (check_malloc(ft_strdup(cmd->value[1])));
		else
			return (old_pwd(cmd, oldpwd));
	}
	else
		return (home_path(home, cmd));
}

int	cd(t_command *cmd, t_global *g)
{
	t_env	*env_pwd;
	char	*path;
	char	*wd;

	if (!check_flag(cmd))
		return (1);
	if (cmd->value[0] && cmd->value[1] && cmd->value[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (g->error_no = 1, 1);
	}
	wd = getcwd(NULL, 0);
	if (!wd)
		wd = ft_strdup(env_finder("PWD")->value);
	wd = check_malloc(wd);
	env_pwd = sync_env(&g->env, "PWD", wd);
	if (!env_pwd)
		error_program(0, 1);
	path = get_path(cmd, env_finder("OLDPWD"), env_finder("HOME"));
	cd_sync(g, path, env_pwd->value, NULL);
	g->the_env = list_to_char(g->env);
	return (1);
}
