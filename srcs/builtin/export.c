/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:58:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/21 13:40:38 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (cmd->value[i][0] == '_')
		return (1);
	str = check_malloc(ft_substr(cmd->value[i], 0, key - cmd->value[i]));
	if (!ft_strlen(key))
		value = check_malloc(ft_strdup(" "));
	else
		value = check_malloc(ft_strdup(key + 1));
	sync_env(&g->env, str, value);
	return (1);
}

static int	export_command(t_command *cmd, t_global *g, int i)
{
	char	*key;
	t_env	*env;

	while (cmd->value[++i] != NULL)
	{
		key = ft_strchr(cmd->value[i], '=');
		if (!key)
			key = cmd->value[i] + ft_strlen(cmd->value[i]);
		env = env_finder(cmd->value[i]);
		if (ft_isdigit(cmd->value[i][0]) || cmd->value[i][0] == '=')
		{
			valid_identifier(cmd, g, i);
			continue ;
		}
		else if (!check_alnum(cmd->value[i], key - cmd->value[i]))
		{
			valid_identifier(cmd, g, i);
			continue ;
		}
		else if (key)
			export_sync_env(cmd, g, key, i);
		else if (!env)
			add_env(&g->env, cmd->value[i], NULL);
	}
	return (g->error_no = 0, 0);
}

static void	export_declare(t_list *list, int fd)
{
	t_env	*env;
	t_list	*print;

	if (!list)
		return ;
	print = list;
	while (print)
	{
		env = (t_env *)print->content;
		if (env->key && env->key[0] != '_')
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(env->key, fd);
			if (env->value && env->value[0] != ' ')
			{
				ft_putstr_fd("=\"", fd);
				ft_putstr_fd(env->value, fd);
				ft_putstr_fd("\"\n", fd);
			}
			else
				ft_putstr_fd("\n", fd);
		}
		print = print->next;
	}
}

int	export(t_command *cmd, t_global *g)
{
	t_list	*export;

	export = NULL;
	if (!check_flag(cmd))
		return (g->error_no = 1, 1);
	if (export_command(cmd, g, 0))
		return (0);
	g->the_env = list_to_char(g->env);
	export = msh_lstcpy(g->env);
	msh_lstsort(&export, NULL, NULL);
	if (!cmd->value[1])
		export_declare(export, cmd->fd[1]);
	return (1);
}
