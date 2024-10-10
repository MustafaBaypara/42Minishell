/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:58:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/10 20:30:56 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	valid_identifier(t_command *cmd, t_global *g, int i)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(cmd->value[i], 2);
	ft_putendl_fd(" : not a valid identifier", 2);
	g->error_no = 1;
	return (0);
}

static int	export_sync_env(t_command *cmd, t_global *g, char *key, int i)
{
	char	*str;
	char	*value;
	int		res;

	str = check_malloc(ft_substr(cmd->value[i], 0, key - cmd->value[i]));
	value = check_malloc(ft_strdup(key + 1));
	res = sync_env(&g->env, key, value);
	return (res);
}

static int	export_command(t_command *cmd, t_global *g)
{
	int		i;
	char	*key;
	t_env	*env;

	i = 0;
	while (cmd->value[++i])
	{
		key = ft_strchr(cmd->value[i], '=');
		if (!key)
			key = cmd->value[i] + ft_strlen(cmd->value[i]);
		env = env_finder(cmd->value[i]);
		if (ft_isdigit(cmd->value[i][0]) || cmd->value[i][0] == '=')
			return (valid_identifier(cmd, g, i));
		else if (!check_alnum(cmd->value[i], key - cmd->value[i]))
			return (valid_identifier(cmd, g, i));
		else if (key)
			export_sync_env(cmd, g, key, i);
		else if (!env)
			add_env(&g->env, cmd->value[i], NULL);
	}
	return (g->error_no = 0, 0);
}

static void	export_declare(t_env *env, int fd)
{
	if (!env)
		return ;
	while ()
}

int export(t_command *cmd, t_global *g)
{
	t_env	*export;

	export = NULL;
	if (!check_flag(cmd))
		return (g->error_no = 1, 1);
	if (export_command(cmd, g))
		return (0);
	g->the_env = list_to_char(g->env);
	if (!cmd->value[1])
		export_declare(export, cmd->fd[1]);
	return (1);
}
