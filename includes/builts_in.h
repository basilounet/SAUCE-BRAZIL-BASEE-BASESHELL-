/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builts_in.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 14:17:23 by gangouil          #+#    #+#             */
/*   Updated: 2024/04/12 13:48:34 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTS_IN_H
# define BUILTS_IN_H

# include <dirent.h>
# include <sys/stat.h>

typedef struct s_env
{
	char			*name;
	char			*var;
	struct s_env	*next;
}					t_env;

char				*wildcards(t_env *env, char *wc);
int					cd_set_pwd(t_env **env);
char				*check_cdpath(t_env **env, char *directory);
void				set_interactive_mode(int set);
int					print_export(t_env *env);
int					is_evenly_quoted(char *str, int n);
int					env_array_to_list(t_env **env, char **char_env);
int					export(t_env **env, char **args);
void				echo(char **args);
t_env				*ft_envnew(char *name, char *var, t_env *next);
t_env				*ft_envlast(t_env *stack);
char				*get_env_var(t_env *env, char *name);
void				ft_envdel_one(t_env *stack);
void				unset(t_env **env, char **args);
void				unset_name(t_env **env, char *name);
void				ft_envadd_back(t_env **stack, t_env *new);
void				ft_envclear(t_env *stack);
char	**env_list_to_array(t_env *env);

#endif