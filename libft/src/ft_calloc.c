/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 13:52:45 by marvin            #+#    #+#             */
/*   Updated: 2024/03/14 17:30:39 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*array;
	size_t			len;

	len = nmemb * size;
	if (len > 2147483647 || (nmemb > 65535 && size > 65535))
		return (NULL);
	array = (unsigned char *)malloc(nmemb * size);
	if (array == NULL)
		return (NULL);
	ft_bzero(array, nmemb * size);
	return (array);
}
