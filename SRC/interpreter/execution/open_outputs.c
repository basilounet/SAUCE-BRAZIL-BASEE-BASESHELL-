/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_outputs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:24:53 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/15 14:04:30 by bvasseur         ###   ########.fr       */
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
	while (ft_strcmp(str, *stop))
	{
		str = ft_str_reajoin(str, "\n", 1, 0);
		ft_putstr_fd(str, fd);
		ft_free_ptr(1, str);
		str = readline("> ");
	}
	if (!str)
		ft_printf("baseshell: warning: here-document delimited by end-of-file \
(wanted `%s')\n", *stop);
	close(fd);
	ft_free_ptr(1, str);
}
