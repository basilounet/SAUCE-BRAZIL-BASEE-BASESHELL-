/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:36:42 by gangouil          #+#    #+#             */
/*   Updated: 2024/03/24 20:59:32 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.h>
#include <stdlib.h>

t_node	*ft_nodenew(e_type type, t_command cmd, t_tree tree)
{
	t_node	*stack;

	stack = ft_calloc(sizeof(t_node), 1);
	if (!stack)
		return (NULL);
	stack->type = type;
	if (type == T_CMD)
		stack->cmd = cmd;
	else
		stack->tree = tree;
	return (stack);
}

t_command	ft_cmdnew(t_tokens *args, t_tokens *redirects)
{
	t_command	cmd;

	cmd.args = args;
	cmd.redirects = redirects;
	return (cmd);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->type == T_CMD)
	{	
		ft_tokclear(&node->cmd.args);
		ft_tokclear(&node->cmd.redirects);
		node->cmd.args = NULL;
		node->cmd.redirects = NULL;
		free(node);
		return ;
	}
	ft_tokclear(&node->tree.redirects);
	if (node->tree.left)
		free_node(node->tree.left);
	if (node->tree.right)
		free_node(node->tree.right);
	node->tree.redirects = NULL;
	node->tree.left = NULL;
	node->tree.right = NULL;
	free(node);
}