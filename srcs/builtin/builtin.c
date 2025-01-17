/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:57:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/19 18:48:07 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_flag(t_command *c)
{
	if (!c->value[1])
		return (1);
	if (c->value[1][0] == '-' && (ft_strlen(c->value[1]) > 1))
	{
		ft_putendl_fd("minishell: flag is not expected.", c->fd[1]);
		return (0);
	}
	return (1);
}

static int	execute_builtin(t_command *cmd, int num,
	int (*f)(t_command *cmd, t_global *g))
{
	t_global	*g;

	g = _global(NULL);
	if (!(*f)(cmd, g))
		return (error_program(0, g->error_no), 1);
	if (num)
		error_program(0, g->error_no);
	return (0);
}

int	builtin_check(t_command *cmd, int num)
{
	if (!cmd->value[0])
		return (1);
	if (!ft_strcmp(cmd->value[0], "echo"))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strcmp(cmd->value[0], "cd"))
		return (execute_builtin(cmd, num, cd));
	else if (!ft_strcmp(cmd->value[0], "pwd"))
		return (execute_builtin(cmd, num, pwd));
	else if (!ft_strcmp(cmd->value[0], "env"))
		return (execute_builtin(cmd, num, env));
	else if (!ft_strcmp(cmd->value[0], "unset"))
		return (execute_builtin(cmd, num, unset));
	else if (!ft_strcmp(cmd->value[0], "export"))
		return (execute_builtin(cmd, num, export));
	else if (!ft_strcmp(cmd->value[0], "exit"))
		return (execute_builtin(cmd, num, exit_func));
	return (1);
}

t_list	*msh_lstcpy(t_list *lst)
{
	t_list	*new;
	t_list	*current;
	t_list	*env;

	new = NULL;
	env = NULL;
	current = lst;
	while (current)
	{
		new = check_malloc(ft_lstnew(current->content));
		ft_lstadd_back(&env, new);
		current = current->next;
	}
	return (env);
}

void	msh_lstsort(t_list **lst, t_env *env, t_env *tmp_env)
{
	t_list	*tmp;
	t_list	*node;
	void	*content;
	int		i;

	i = 0;
	node = *lst;
	while (node)
	{
		env = (t_env *)node->content;
		tmp = node->next;
		while (tmp)
		{
			tmp_env = (t_env *)tmp->content;
			i = ft_strlen(env->key);
			if (ft_strncmp(env->key, tmp_env->key, i) > 0)
			{
				content = node->content;
				node->content = tmp->content;
				tmp->content = content;
			}
			tmp = tmp->next;
		}
		node = node->next;
	}
}
