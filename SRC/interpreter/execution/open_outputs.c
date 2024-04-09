/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_outputs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:24:53 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/08 19:30:05 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	unlink_here_docs(t_ms *ms)
{
	char	*name;
	int		i;

	i = 0;
	name = NULL;
	while (i < ms->heredoc_number)
	{
		name = ft_str_reajoin("here_doc_", ft_itoa(i), 0, 1);
		if (name)
		{
			unlink(name);
			free(name);
		}
		i++;
	}
}

void	get_new_file(t_ms *ms, char **stop)
{
	char	*str;
	char	*history;
	int		fd;

	str = expand_var(ms->env, *stop, (t_expand_args){0});
	ft_free_ptr(1, *stop);
	*stop = str;
	str = ft_str_reajoin("here_doc_", ft_itoa(ms->heredoc_number), 0, 1);
	fd = open(str, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	ft_free_ptr(1, str);
	if (fd < 0)
		return ;
	str = readline("> ");
	history = ft_strdup(str);
	while (ft_strcmp(str, *stop))
	{
		add_history(history);
		str = ft_str_reajoin(str, "\n", 1, 0);
		ft_putstr_fd(str, fd);
		ft_free_ptr(1, str);
		str = readline("> ");
		history = ft_str_reajoin(history, "\n", 1, 0);
		history = ft_str_reajoin(history, str, 1, 0);
	}
	if (!str)
		ft_printf("baseshell: warning: here-document delimited by end-of-file \
(wanted `%s')\n", *stop);
	close(fd);
	ft_free_ptr(1, str);
}

static void	open_redirects(t_tokens *redirects)
{
	(void)ms;
	while (redirects)
	{
		if (redirects->symbol == T_OUTPUT)
			try_close_fd(open(redirects->arg, O_CREAT | O_WRONLY | O_TRUNC,
					0644));
		if (redirects->symbol == T_APPEND)
			try_close_fd(open(redirects->arg, O_CREAT | O_WRONLY | O_APPEND,
					0777));
		redirects = redirects->next;
	}
}

void	open_all_outputs(t_ms *ms, t_node *node)
{
	if (!node)
		return ;
	if (node->type == T_TREE)
	{
		open_all_outputs(ms, node->tree.left);
		open_all_outputs(ms, node->tree.right);
		open_redirects(node->tree.redirects);
	}
	else
		open_redirects(node->cmd.redirects);
}
