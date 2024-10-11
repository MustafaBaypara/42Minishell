/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 16:57:24 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/11 18:18:40 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		return (error_program(0, 12), 1);
	if (num)
		error_program(0, g->error_no);
	return (0);
}

int	builtin_check(t_command *cmd, int num)
{
	if (!cmd->value[0])
		return (1);
	if (!ft_strncmp(cmd->value[0], "echo", ft_strlen("echo")))
		return (execute_builtin(cmd, num, echo));
	else if (!ft_strncmp(cmd->value[0], "cd", ft_strlen("cd")))
		return (execute_builtin(cmd, num, cd));
	else if (!ft_strncmp(cmd->value[0], "pwd", ft_strlen("pwd")))
		return (execute_builtin(cmd, num, pwd));
	else if (!ft_strncmp(cmd->value[0], "env", ft_strlen("env")))
		return (execute_builtin(cmd, num, env));
	else if (!ft_strncmp(cmd->value[0], "unset", ft_strlen("unset")))
		return (execute_builtin(cmd, num, unset));
	else if (!ft_strncmp(cmd->value[0], "export", ft_strlen("export")))
		return (execute_builtin(cmd, num, export));
	else if (!ft_strncmp(cmd->value[0], "exit", ft_strlen("exit")))
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

void	msh_lstsort(t_list **lst)
{
	t_list	*tmp;
	t_list	*node;
	void	*content;
	int		i;

	i = 0;
	node = *lst;
	while (node)
	{
		tmp = node->next;
		while (tmp)
		{
			i = ft_strlen(node->content);
			if (ft_strncmp(node->content, tmp->content, i) > 0)
			{
				content = node->content;
				node->content = tmp->content;
				tmp->content = content;
			}
			tmp = tmp->next;
		}
	}
}
