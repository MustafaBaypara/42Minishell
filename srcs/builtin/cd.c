/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:27:16 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 12:42:04 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

static char	*home_path(t_env *home, t_command *cmd)
{
	char	*path;

	if (home)
	{
		path = check_malloc(ft_strdup(home->value)); // home değerini alır
		return (path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", cmd->fd[1]); // ayarlı değilse hata
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
		ft_putendl_fd(path, cmd->fd[1]); // oldpwd yazdırır
		return (path);
	}
	else
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", cmd->fd[1]); // ayarlı değilse hata
		_global(NULL)->error_no = 1;
		return (check_malloc(ft_calloc(1, 1)));
	}
}

static char	*get_path(t_command *cmd, t_env *oldpwd, t_env *home)
{
	if (cmd->value[1])
	{
		if (cmd->value[1][0] != '-')
			return (check_malloc(ft_strdup(cmd->value[1]))); // "cd -" yoksa old pwd almaz
		else
			return (old_pwd(cmd, oldpwd));
	}
	else
		return (home_path(home, cmd)); // "cd " şeklindeyse home alır
}

int	cd(t_command *cmd, t_global *g)
{
	t_env	*env_pwd;
	char	*path;
	char	*wd;

	if (!check_flag(cmd)) // flag kontrolü
		return (1);
	if (cmd->value[0] && cmd->value[1] && cmd->value[2]) // argüman kontrolü
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (g->error_no = 1, 1);
	}
	wd = getcwd(NULL, 0); // çalışma dizinini alır
	if (!wd)
		wd = ft_strdup(env_finder("PWD")->value); // yoksa PWD'yi alır
	wd = check_malloc(wd);
	env_pwd = sync_env(&g->env, "PWD", wd);
	if (!env_pwd)
		error_program(0, 1);
	path = get_path(cmd, env_finder("OLDPWD"), env_finder("HOME")); // path'i alır
	cd_sync(g, path, env_pwd->value, NULL); // günceller
	g->the_env = list_to_char(g->env); // the_env'i günceller
	return (1);
}
