/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvasseur <bvasseur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 14:47:49 by bvasseur          #+#    #+#             */
/*   Updated: 2024/04/15 21:23:44 by bvasseur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <parser.h>
#include <builts_in.h>

static char	*sym_to_char(t_symbol symbol)
{
	if (symbol == T_PIPE)
		return ("|");
	if (symbol == T_INPUT)
		return ("<");
	if (symbol == T_HEREDOC)
		return ("<<");
	if (symbol == T_OUTPUT)
		return (">");
	if (symbol == T_APPEND)
		return (">>");
	if (symbol == T_LPARENTH)
		return ("(");
	if (symbol == T_RPARENTH)
		return (")");
	if (symbol == T_OR)
		return ("||");
	if (symbol == T_AND)
		return ("&&");
	return (NULL);
}

void	print_pad(int depth)
{
	while (depth)
	{
		printf("|  ");
		depth--;
	}
}

void	print_tokens(t_tokens *tokens, int depth)
{
	while (tokens)
	{
		print_pad(depth);
		printf("symbol = %u, arg = \"%s\"\n", tokens->symbol, tokens->arg);
		tokens = tokens->next;
	}
}

void	print_node(t_node *node, int depth)
{
	if (!node)
		return ;
	if (node->type == T_CMD)
	{
		print_pad(depth);
		printf("<cmd>\n");
		print_pad(++depth);
		printf("<args>\n");
		print_tokens(node->cmd.args, depth);
		print_pad(++depth - 1);
		printf("<redirects>\n");
		print_tokens(node->cmd.redirects, depth);
	}
	if (node->type == T_TREE)
	{
		print_pad(depth);
		printf("<tree>\n");
		print_pad(++depth);
		printf("operator = %s\n", sym_to_char(node->tree.operator));
		print_node(node->tree.left, depth + 1);
		print_node(node->tree.right, depth + 1);
		print_pad(depth);
		printf("<redirects>\n");
		print_tokens(node->tree.redirects, depth);
	}
}


/*static char *test[] = {"bla =bli", "bloups", "blagz=", \
	" ", "\"blarbouste\"", "zg\"oug", "bi\"z\"bou", "bi\"z'\"baz", \
	"blax='", "blax=''", "bist=ouri", "bist==ouri", "blorgux=test", \
	"bi\"s\"carosse", "", "bip=", "biap=titou", "bop=\"\"", "bap", \
	"miaousse=\'\"\'$USER\'\"\'", "_zblox=b", "1two3=", "one23=", \
	"b_3=", "bip=swag", "A=\"guy2bezbar\"", "A", "bix=", "bix+=bloarg", \
	"biop", "biop+=$bip", "moufette", NULL};
static char *test2[] = {"export", NULL};
static char *test3[] = {"c*", "*", "t*", "*a*", "*ak*", "**", "*s", \
	"*cases", "cases*", "cas*es", "case*s", "c*ases", NULL};*/

void	ft_free_ms(t_ms *ms)
{
	free_node(ms->root_node);
	ft_tokclear(&ms->tokens);
	ft_free_ptr(1, ms->prompt);
	ft_envclear(ms->env);
	ft_free_map(ms->envp, ft_maplen(ms->envp));
	ft_free_map(ms->char_env, ft_maplen(ms->char_env));
	if (ms->pids)
		free(ms->pids);
}

static void	temp_execution(t_ms *ms, char *line)
{
	t_node		*node;

	ms->tokens = NULL;
	ms->heredoc_number = 0;
	if (lexer(ms, &ms->tokens, line) == 0)
	{
		ft_tokclear(&ms->tokens);
		free(line);
		return ; //malloc error
	}
	node = parse_prompt(ms, &ms->tokens);
	if (!node)
	{
		ft_tokclear(&ms->tokens);
		free(line);
		return ;
	}
	prepare_and_execute(ms, node);
	printf("exit code = %d\n", ms->exit_code);
	node = free_node(node);
	ft_tokclear(&ms->tokens);
	if (line)
		free(line);
	return ;
}

int g_sig;

int	main(int ac, char **av, char **char_env)
{
	t_ms		ms;

	(void)ac;
	(void)av;
	ft_memset((void *)&ms, 0, sizeof(t_ms));
	if (env_array_to_list(&(ms.env), char_env) == 0)
		return (1);
	ms.prompt = add_colors(get_prompt(ms.env), &moving_rainbow_pattern);
	set_interactive_mode(1);
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
	ft_free_ms(&ms);
}
