/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:47:49 by bvasseur          #+#    #+#             */
/*   Updated: 2024/03/16 17:00:10 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

char	*moving_rainbow_pattern(int i, int len)
{
	static int	j = 0;

	if (i >= len - 1)
		j = (j + 1) % len;
	i = (i + j) % len;
	if (i <= len * 1 / 5)
		return (MAGENTA);
	else if (i <= len * 2 / 5)
		return (BLUE);
	else if (i <= len * 3 / 5)
		return (GREEN);
	else if (i <= len * 4 / 5)
		return (YELLOW);
	else if (i <= len)
		return (RED);
	return (BASE_COLOR);
}

char	*moving_france_pattern(int i, int len)
{
	static int	j = 0;

	if (i >= len - 1)
		j = (j + 1) % len;
	i = (i + j) % len;
	if (i <= len * 1 / 3)
		return (BLUE);
	else if (i <= len * 2 / 3)
		return (WHITE);
	else if (i <= len)
		return (RED);
	return (BASE_COLOR);
}

char	*add_colors(char *str, char *(*color_pattern)(int, int))
{
	char	*colored_str;
	int		len;
	int		i;

	i = -1;
	len = ft_strlen(str);
	colored_str = NULL;
	if (!str || !color_pattern)
		return (NULL);
	while (++i < len)
		colored_str = ft_str_reajoin(colored_str,
				ft_str_reajoin(color_pattern(i, len), ft_substr(str, i, 1), 0,
					1), 1, 1);
	colored_str = ft_str_reajoin(colored_str, BASE_COLOR, 1, 0);
	free(str);
	return (colored_str);
}

char	*get_prompt(t_ms *ms)
{
	char	*home;
	char	*pwd;
	char	*str;
	int		i;

	i = 0;
	pwd = ft_getenv(ms->env, "PWD");
	home = ft_getenv(ms->env, "HOME");
	str = ft_strdup("gangouil@minishell:");
	if (!str)
		return (NULL);
	if (!pwd)
		pwd = getcwd(NULL, i++);
	if ((home && pwd && !ft_strncmp(home, pwd, ft_strlen(home)
				&& home[ft_strlen(home)] != '/') && (pwd[ft_strlen(home)] == '/'
				|| pwd[ft_strlen(home)] == '\0')))
		str = ft_str_reajoin(str, ft_strjoin3("~", pwd + ft_strlen(home), "$"),
				1, 1);
	else
		str = ft_str_reajoin(str, ft_strjoin(pwd, "$"), 1, 1);
	if (i && pwd)
		free(pwd);
	return (str);
}

int	main(int ac, char **av, char **env)
{
	t_ms	ms;
	int		i;

	(void)ac;
	(void)av;
	ms.env = ft_mapcpy(env, ft_maplen(env));
	// ft_print_map(ms.env);
	i = -1;
	while (++i < 5000)
	{
		ft_printf("\n");
		ft_put_malloc_free(add_colors(get_prompt(&ms), &moving_rainbow_pattern),
			&free);
		usleep(50000);
	}
	ft_free_map(ms.env, ft_maplen(ms.env));
}
