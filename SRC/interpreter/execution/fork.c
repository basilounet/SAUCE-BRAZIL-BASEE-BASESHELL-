/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:50:01 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/11 13:57:07 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	try_close_fd(int fd)
{
	if (fd > 2)
		close(fd);
}

int	execute_built_ins(t_execution execution, t_node *node)
{
	int	std_out;
	
	if (!execution.is_in_pipe)
	{
		execution.input = get_input_fd(node->cmd.redirects);
		execution.output = get_output_fd(node->cmd.redirects);
		if (node->cmd.args)
			check_command(execution.ms, &node->cmd.args->arg);
		transform_to_chars(execution.ms, node);
		std_out = dup(STDOUT_FILENO);
		if (execution.output >= 0)
			dup2(execution.output, STDOUT_FILENO);
	}
	if (!ft_strcmp(node->cmd.args->arg, "echo"))
		echo(node->cmd.char_args);
	else if (!ft_strcmp(node->cmd.args->arg, "cd"))
		cd(&execution.ms->env, node->cmd.char_args);
	else if (!ft_strcmp(node->cmd.args->arg, "pwd"))
		pwd();
	else if (!ft_strcmp(node->cmd.args->arg, "export"))
	{
		export(&execution.ms->env, node->cmd.char_args);
		reset_envp(execution.ms);
	}
	else if (!ft_strcmp(node->cmd.args->arg, "unset"))
	{
		unset(&execution.ms->env, node->cmd.char_args);
		reset_envp(execution.ms);
	}
	else if (!ft_strcmp(node->cmd.args->arg, "n"))
		env(execution.ms->env);
	else
		return (0);
	if (execution.is_in_pipe)
		exit(execution.ms->exit_code);
	dup2(std_out, STDOUT_FILENO);
	try_close_fd(execution.input);
	try_close_fd(execution.output);
	close(std_out);
	return (1);
}

void	child(t_execution execution, t_node *node)
{
	ft_printf("left_pipe [%d,%d], right_pipe [%d,%d], input:%d, output:%d\n", execution.left_pipe[0], execution.left_pipe[1], execution.right_pipe[0], execution.right_pipe[1], execution.input, execution.output);
	execution.input = get_input_fd(node->cmd.redirects);
	execution.output = get_output_fd(node->cmd.redirects);
	if (node->cmd.args)
		check_command(execution.ms, &node->cmd.args->arg);
	transform_to_chars(execution.ms, node);
	if (execution.input >= 0)
		dup2(execution.input, STDIN_FILENO);
	else if (execution.left_pipe[READ] >= 0)
		dup2(execution.left_pipe[READ], STDIN_FILENO);
	if (execution.output >= 0)
		dup2(execution.output, STDOUT_FILENO);
	else if (execution.right_pipe[WRITE] >= 0)
		dup2(execution.right_pipe[WRITE], STDOUT_FILENO);
	try_close_fd(execution.input);
	try_close_fd(execution.output);
	try_close_fd(execution.left_pipe[WRITE]);
	try_close_fd(execution.left_pipe[READ]);
	try_close_fd(execution.right_pipe[WRITE]);
	try_close_fd(execution.right_pipe[READ]);
	if (node->cmd.args && (!execution.ms->exit_code || (!(is_built_in(node->cmd.args->arg) && execute_built_ins(execution, node)))))
		execve(node->cmd.char_args[0], node->cmd.char_args, execution.ms->envp);
	free_node(execution.ms->root_node);
	ft_tokclear(&execution.ms->tokens);
	free(execution.ms->prompt);
	ft_envclear(execution.ms->env);
	ft_free_map(execution.ms->envp, ft_maplen(execution.ms->envp));
	if (execution.ms->pids)
		free(execution.ms->pids);
	exit(execution.ms->exit_code);
}

void	parent(t_execution execution)
{
	try_close_fd(execution.input);
	try_close_fd(execution.output);
	try_close_fd(execution.right_pipe[WRITE]);
	try_close_fd(execution.left_pipe[READ]);
}

/*
cat a | cat -e | cat -e | cat -e
*/