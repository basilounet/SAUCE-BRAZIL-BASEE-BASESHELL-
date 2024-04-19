/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:47:49 by bvasseur          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/04/17 21:07:38 by bvasseur         ###   ########.fr       */
=======
/*   Updated: 2024/04/17 13:13:25 by bvasseur         ###   ########.fr       */
>>>>>>> refs/remotes/origin/main
/*                                                                            */
/* ************************************************************************** */

#include <builts_in.h>
#include <minishell.h>
#include <parser.h>

void	ft_free_ms(t_ms *ms)
{
	//ft_tokclear(&ms->tokens);
	ft_free_ptr(1, ms->prompt);
	ft_envclear(ms->env);
	ft_free_map(ms->envp, ft_maplen(ms->envp));
	ft_free_map(ms->char_env, ft_maplen(ms->char_env));
	free_node(ms->root_node);
	if (ms->pids)
		free(ms->pids);
}

static void	temp_execution(t_ms *ms, char *line)
{
	t_node	*node;

	ms->tokens = NULL;
	ms->heredoc_number = 0;
	if (lexer(ms, &ms->tokens, line) == 0)
	{
		ft_tokclear(&ms->tokens);
		free(line);
		return ;
	}
	if (!ms->tokens)
		return ;
	node = parse_prompt(ms, &ms->tokens);
	if (!node)
	{
		ft_tokclear(&ms->tokens);
		free(line);
		return ;
	}
	prepare_and_execute(ms, node);
	//printf("exit code = %d\n", ms->exit_code);
	node = free_node(node);
	ft_tokclear(&ms->tokens);
	if (line)
		free(line);
	return ;
}

int			g_sig;

/*static char *test[] = {"bla =bli", "bloups", "blagz=", \
	" ", "\"blarbouste\"", "zg\"oug", "bi\"z\"bou", "bi\"z'\"baz", \
	"blax='", "blax=''", "bist=ouri", "bist==ouri", "blorgux=test", \
	"bi\"s\"carosse", "", "bip=", "biap=titou", "bop=\"\"", "bap", \
	"miaousse=\'\"\'$USER\'\"\'", "_zblox=b", "1two3=", "one23=", \
	"b_3=", "bip=swag", "A=\"guy2bezbar\"", "A", "bix=", "bix+=bloarg", \
	"biop", "biop+=$bip", "moufette", NULL};
static char *test2[] = {"export", NULL};
static char *test3[] = {"*s", "*a*", "cas*es", "c*", "*", "t*", "*ak*", "**", \
	"*cases", "cases*", "case*s", "c*ases", NULL};
static char *test4[] = {"*\"t\"", "*d\'e\'\"\"s", "t\'*\'", "\"\'t\'\"*", "\'t\'*", NULL};
*/

int	main(int ac, char **av, char **char_env)
{
	t_ms	ms;
	int		exit_code;

	(void)ac;
	(void)av;
	ft_memset((void *)&ms, 0, sizeof(t_ms));
	if (env_array_to_list(&(ms.env), char_env) == 0)
		return (1);
	ms.prompt = add_colors(get_prompt(ms.env), &moving_rainbow_pattern);
	while (1)
	{
		set_interactive_mode(1);
		ms.line = readline(ms.prompt);
		if (!ms.line)
			break ;
		if (ms.line[0] != '\0')
			add_history(ms.line);
		temp_execution(&ms, ms.line);
		free(ms.prompt);
		ms.prompt = add_colors(get_prompt(ms.env), &moving_rainbow_pattern);
	}
	rl_clear_history();
	exit_code = ms.exit_code;
	ft_free_ms(&ms);
	return (exit_code);
}
