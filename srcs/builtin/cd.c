/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 16:27:16 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/09 16:46:18 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int cd(t_command *cmd, t_global *g)
{
	t_list	*env_pwd;
	char	*path;
	char	*wd;

	if (!check_flag(cmd))
		return (1);
	wd = check_malloc(getcwd(NULL, 0));
	env_pwd = sync_env(&g->env, "PWD", wd);
}
