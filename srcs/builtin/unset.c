/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbaypara <mbaypara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 17:04:34 by mbaypara          #+#    #+#             */
/*   Updated: 2024/10/11 18:02:02 by mbaypara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	export_del(t_list **lst, char *str)
{
	t_list	*tmp;
	t_list	*prev;
	t_env	*env;

	tmp = *lst;
	prev = NULL;
	if (!tmp)
		return (1); // Liste boşsa 1 döner

	env = (t_env *)tmp->content;
	if (ft_strncmp(env->key, str, ft_strlen(str)) == 0)
	{
		*lst = tmp->next; // Baş düğüm siliniyorsa başı güncelle
		// Burada garbage collector ile bellek yönetimi
		return (0); // Silme başarılı
	}

	while (tmp)
	{
		env = (t_env *)tmp->content;
		if (ft_strncmp(env->key, str, ft_strlen(str)) == 0)
		{
			if (prev)
				prev->next = tmp->next; // Düğüm bulunursa bağlantıyı güncelle
			// Burada garbage collector ile bellek yönetimi
			return (0); // Silme başarılı
		}
		prev = tmp; // Önceki düğümü güncelle
		tmp = tmp->next; // Sonraki düğüme geç
	}
	return (1); // Düğüm bulunamazsa 1 döner
}

int	unset(t_command *cmd, t_global *g)
{
	int	i;

	i = 0;
	if (!check_flag(cmd))
		return (g->error_no = 1, 1); // Hatalı bayrak varsa 1 döner

	while (cmd->value[++i]) // cmd->value'daki her elemanı kontrol et
	{
		if (export_del(&g->env, cmd->value[i]) == 0)
			return (0); // Düğüm silindiyse 0 döner
	}
	return (g->error_no = 0, 1); // Tüm işlemler başarılıysa 1 döner
}

