/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gangouil <gangouil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 20:31:02 by gangouil          #+#    #+#             */
/*   Updated: 2024/03/22 20:31:15 by gangouil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <builts_in.h>

static t_env	*parse_char_env(t_env *env, char *char_env, int j, int append)
{
	t_env	*new_env;
	char	*name;
	char	*var;

	name = ft_substr(char_env, 0, j);
	if (append == 0)
		var = ft_substr(char_env, j + 1, ft_strlen(char_env));
	else
		var = ft_str_reajoin(get_env_var(env, name), \
		ft_substr(char_env, j + 2, ft_strlen(char_env)), 1, 1);
	new_env = ft_envnew(name, var, NULL);
	if (!new_env || !name || !var)
	{
		if (name)
			free(name);
		if (var)
			free(var);
		if (new_env)
			free(new_env);
		return (NULL);
	}
	return (new_env);
}

static char	*is_export_valid(t_env *env, char *arg)
{
	int	i;

	i = 0;
	if (!is_evenly_quoted(arg, 0)) // check en fin de minishell si necessaire
		return (NULL);
	//arg = expand_var(env, arg, 0);
	if (!arg || !arg[0] || arg[0] == '=' || (arg[0] >= '0' && arg[0] <= '9'))
	{
		if (arg)
			free(arg);
		return (NULL);
	}
	while (arg[i])
	{
		if (i && ((arg[i] == '=') || (arg[i] == '+' && arg[i + 1 == '='])))
			break ;
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			free(arg);
			return (NULL);
		}
		i++;
	}
	return (arg);
}

static int	get_export_values(t_env **env, char *expand_arg)
{
	int		j;
	char	*name;
	t_env	*new_env;
	
	j = 0;
	while (expand_arg[j] && expand_arg[j] != '=' && expand_arg[j] != '+')
		j++;
	if (!expand_arg[j])
	{
		name = ft_substr(expand_arg, 0, j);
		if (!name)
			return (0); // malloc error
		new_env = ft_envnew(name, NULL, NULL);
	}
	else if (expand_arg[j] == '=')
		new_env = parse_char_env(*env, expand_arg, j, 0);
	else
		new_env = parse_char_env(*env, expand_arg, j, 1);
	if (!new_env)
		return (0); // malloc error
	unset_name(env, new_env->name);
	ft_envadd_back(env, new_env);
	return (1);
}

int	export(t_env **env, char **args)
{
	int		i;
	int		j;
	char	*expand_arg;

	i = 1; // change to 1 after testing
	if (!args[1])
		print_export(*env);
	while (args[i])
	{
		expand_arg = is_export_valid(*env, args[i]);
		if (!expand_arg)
		{
			i++;
			continue ;
		}
		if (!get_export_values(env, expand_arg))
			return (0); // malloc error
		free(expand_arg);
		i++;
	}
	return (1);
}

int	env_array_to_list(t_env **env, char **char_env)
{
	int		i;
	int		j;
	t_env	*new_env;

	i = 0;
	while (char_env[i])
	{
		j = 0;
		while (char_env[i][j] != '=')
			j++;
		new_env = parse_char_env(*env, char_env[i], j, 0);
		if (!new_env)
		{
			ft_envclear(*env); //malloc error
			return (0);
		}
		ft_envadd_back(env, new_env);
		i++;
	}
	return (1);
}