/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:27:16 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/09 18:15:00 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*home_path(t_command *cmd, t_list *home, t_global *g)
{
	char	*path;
	t_env	*tmp;

	tmp = (t_env *)home->content;
	if (home)
	{
		path = check_malloc(ft_strdup(tmp->value));
		return (path);
	}
	else
		return (NULL);
}

static char	*old_pwd(t_command *cmd, t_list *oldpwd, t_global *g)
{
	char	*path;
	t_env	*tmp;

	tmp = (t_env *)oldpwd->content;
	if (oldpwd)
	{
		path = check_malloc(ft_strdup(tmp->value));
		ft_putendl_fd(path, cmd->fd[1]);
		return (path);
	}
	return (NULL);
}

static char	*get_path(t_command *cmd, t_list *oldpwd, t_list *home, t_global *g)
{
	if (cmd->value[1])
	{
		if (cmd->value[1][0] != '-')
			return (check_malloc(ft_strdup(cmd->value[1])));
		else
			return (old_pwd(cmd, oldpwd, g));
	}
	else
		return (home_path(cmd, home, g));
}

int cd(t_command *cmd, t_global *g)
{
	t_list	*env_pwd;
	char	*path;
	char	*wd;

	if (!check_flag(cmd))
		return (1);
	wd = check_malloc(getcwd(NULL, 0));
	env_pwd = sync_env(&g->env, "PWD", wd);
	if (!env_pwd)
	{
		g->error_no = 1;
		error_program(0, g->error_no);
	}
	path = get_path(cmd, env_finder("OLDPWD"), env_finder("HOME"), g);
	cd_sync()
}
