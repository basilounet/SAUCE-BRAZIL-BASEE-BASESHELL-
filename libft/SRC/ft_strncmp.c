/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 14:05:47 by marvin            #+#    #+#             */
/*   Updated: 2024/03/16 14:15:56 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*area1;
	unsigned char	*area2;
	size_t			i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	area1 = (void *)s1;
	area2 = (void *)s2;
	while ((area1[i] || area2[i]) && i < n)
	{
		if (area1[i] != area2[i])
			return (area1[i] - area2[i]);
		i++;
	}
	return (0);
}
