/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 14:54:02 by bvasseur          #+#    #+#             */
/*   Updated: 2024/03/17 14:55:57 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	print_env(t_env	*env)
{
	while (env)
	{
		if (env->var)
			printf("%s=%s\n", env->name, env->var);
		env = env->next;
	}
}