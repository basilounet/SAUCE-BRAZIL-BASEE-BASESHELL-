/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:36:42 by gangouil          #+#    #+#             */
/*   Updated: 2024/03/24 21:10:57 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lexer.h>
#include <stdlib.h>

t_tokens	*ft_toknew(e_symbol symbol, t_tokens *next)
{
	t_tokens	*stack;

	stack = ft_calloc(sizeof(t_tokens), 1);
	if (!stack)
		return (NULL);
	stack->symbol = symbol;
	stack->arg = NULL;
	stack->next = next;
	return (stack);
}

t_tokens	*ft_toklast(t_tokens *stack)
{
	t_tokens	*tmp;

	tmp = stack;
	if (!tmp)
		return (NULL);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	ft_tokadd_back(t_tokens **stack, t_tokens *new)
{
	t_tokens	*last;

	if (!new)
		return ;
	if (!*stack)
	{
		*stack = new;
		return ;
	}
	last = ft_toklast(*stack);
	last->next = new;
}

void	ft_tokadd_front(t_tokens **stack, t_tokens *new)
{
	if (!new || !stack)
		return ;
	ft_toklast(new)->next = *stack;
	*stack = new;
}

void	ft_tokclear(t_tokens **stack)
{
	t_tokens	*tmp;

	while (*stack)
	{
		tmp = (*stack)->next;
		if ((*stack)->arg)
			free((*stack)->arg);
		free(*stack);
		*stack = NULL;
		*stack = tmp;
	}
	*stack = NULL;
}

t_tokens	*ft_tokpop(t_tokens **stack)
{
	t_tokens	*current_token;

	if (!stack || !*stack)
		return (NULL);
	current_token = *stack;
	*stack = (*stack)->next;
	current_token->next = NULL;
	return (current_token);
}

int	ft_toksize(t_tokens *tokens)
{
	t_tokens	*tmp;
	int			i;

	i = 0;
	tmp = tokens;
	while (tmp)
	{
		i++;
		tmp = tmp ->next;
	}
	return (i);
}
