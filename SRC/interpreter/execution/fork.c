/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:50:01 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/15 21:16:22 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void    close_all_fds(t_ms *ms)
{
    int i;

    i = 0;
    while (i < 1024)
        try_close_fd(i++);
    errno = ms->exit_code;
}


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
	else if (!ft_strcmp(node->cmd.args->arg, "env"))
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
	//ft_printf("left_pipe [%d,%d], right_pipe [%d,%d], upper_pipe [%d,%d], input:%d, output:%d\n", execution.left_pipe[0], execution.left_pipe[1], execution.right_pipe[0], execution.right_pipe[1], execution.upper_pipe[0], execution.upper_pipe[1], execution.input, execution.output);
	execution.input = get_input_fd(node->cmd.redirects);
	execution.output = get_output_fd(node->cmd.redirects);
	execution.ms->exit_code = 0;
	if (node->cmd.args)
		check_command(execution.ms, &node->cmd.args->arg);
	transform_to_chars(execution.ms, node);
	if (execution.input >= 0)
		dup2(execution.input, STDIN_FILENO);
	else if (execution.left_pipe[READ] >= 0)
		dup2(execution.left_pipe[READ], STDIN_FILENO);
	else if (execution.lower_pipe[READ] >= 0)
		dup2(execution.lower_pipe[READ], STDIN_FILENO);
	if (execution.output >= 0)
		dup2(execution.output, STDOUT_FILENO);
	else if (execution.right_pipe[WRITE] >= 0)
		dup2(execution.right_pipe[WRITE], STDOUT_FILENO);
	else if (execution.upper_pipe[WRITE] >= 0)
		dup2(execution.upper_pipe[WRITE], STDOUT_FILENO);
	//print_fds(execution.all_pipes);
	close_all_fds(execution.ms);
	if (node->cmd.args && !execution.ms->exit_code && !(is_built_in(node->cmd.args->arg) && execute_built_ins(execution, node)))
		execve(node->cmd.char_args[0], node->cmd.char_args, execution.ms->char_env);
	exit(execution.ms->exit_code);
}

void	parent(t_execution execution)
{
	try_close_fd(execution.input);
	try_close_fd(execution.output);
	try_close_fd(execution.right_pipe[WRITE]);
	try_close_fd(execution.left_pipe[READ]);
}
