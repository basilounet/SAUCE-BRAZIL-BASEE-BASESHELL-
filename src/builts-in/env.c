/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:54:02 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/08 13:37:32 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:36:42 by gangouil          #+#    #+#             */
/*   Updated: 2024/03/24 20:58:57 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	env(t_env	*env)
{
	while (env)
	{
		if (env->var)
			printf("%s=%s\n", env->name, env->var);
		env = env->next;
	}
}