/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oantonen <oantonen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/29 13:20:44 by oantonen          #+#    #+#             */
/*   Updated: 2018/04/18 21:57:17 by oantonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core_asm.h"

// void	split_errors()

void 	save_str(int type, char *str, int *k, t_spl *i_spl)
{
	char 	*s;
	char 	**args;

	if (type == 1)
	{
		// ft_printf("k=%d\n", *k);
		// ft_printf("incom_str=%s\n", str);
		i_spl->lbl = ft_strsub(str - *k + 1, 0, *k - 1);
	}
	else if (type == 2)
	{
		// ft_printf("k=%d\n", *k);
		// ft_printf("incom_str=%s\n", str);
		i_spl->op_code = ft_strsub(str - *k, 0, *k);
		args = ft_strsplit(str, ',');
		*k = 0;
		while (args[*k])
		{
			s = ft_strtrim(args[*k]);
			ft_list_push_back(&(i_spl->args), ft_lstnew(s, 0));
			ft_strdel(&args[*k]);
			(*k)++;
		}
		i_spl->q_arg = *k;
		free(args);
	}
	*k = 0;
}

int		token(char c, char *str, int k, t_spl *i_spl)//пропускает говно?
{
	if (c == LABEL_CHAR && i_spl->lbl == NULL)
		return (1);
	else if ((c == ' ' || c == '\t') && !i_spl->op_code && k != 0)
		return (2);
	return (0);
}

void	split_cur_line(t_list **bgng, char *str, int ln_nb)
{
	int 	i;
	int 	k;
	int 	type;
	t_spl	*i_spl;

	i = 0;
	k = 0;
	// ft_printf("str=%s\n", str);
	i_spl = (t_spl*)ft_memalloc(sizeof(t_spl));
	i_spl->ln_nb = ln_nb;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			k++;
		type = token(str[i], &str[i], k, i_spl);
		if (type)
			save_str(type, &str[i], &k, i_spl);
		if (type == 2)
			break ;
		i++;
	}
	ft_list_push_back(bgng, ft_lstnew(i_spl, 0));
}

void	split_lines(t_fls *file, t_list *instr)
{
	int 	i;
	t_list	*spl_i;

	spl_i = NULL;
	i = 0;
	while (instr)
	{
		// ft_printf("line=%s\n", (char*)instr->content);
		split_cur_line(&spl_i, instr->content, instr->content_size);
		instr = instr->next;
	}
	t_list *ptr = spl_i;
	t_list *arg;
	char *s;
	i = 0;
	while (ptr)
	{
		// ft_printf("%d\n", i++);
		arg = ((t_spl*)ptr->content)->args;
		s = ((t_spl*)ptr->content)->op_code;
		// if (((t_spl*)ptr->content)->lbl)
			// ft_printf("lbl=%s\n", ((t_spl*)ptr->content)->lbl);
		ft_printf("instr=%s\n", s);
		while (arg)
		{
			// ft_printf("arg=%s\n", (char*)arg->content);
			arg = arg->next;
		}
		ptr = ptr->next;
	}
	file->spltd = spl_i;
}








