/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_instructions2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oantonen <oantonen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 20:56:18 by oantonen          #+#    #+#             */
/*   Updated: 2018/04/26 14:21:07 by oantonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core_asm.h"
#include "op.h"

int		check_indirect(char *arg, int type)
{
	int		i;

	i = 0;
	while (arg[i] && (ft_isdigit(arg[i]) || arg[0] == '-'))
		i++;
	if (arg[i] == '\0' && type == 1)
		return (2);
	else if (arg[i] == '\0' && type == 2)
		return (4);
	return (0);
}

int		check_direct(char *arg, t_list *lbls)
{
	t_list	*ptr;

	ptr = lbls;
	if (arg[1] == LABEL_CHAR)
	{
		while (ptr)
		{
			if (ft_strequ(&arg[2], ((t_lbl*)ptr->content)->name))
				break ;
			ptr = ptr->next;
		}
		if (ptr != NULL)
			return (2);
	}
	else
		return (check_indirect(&arg[1], 1));
	return (0);
}

int		identify_argtype(char *arg, t_list *lbls)
{
	int		nb;
	char	*tmp;

	nb = 0;
	// ft_printf("arg=%s\n", arg);
	if (arg[0] == 'r')
	{
		nb = ft_atoi(&arg[1]);
		tmp = ft_itoa(nb);
		if (nb >= 1 && nb < REG_NUMBER && ft_strequ(&arg[1], tmp))
		{
			free(tmp);
			return (1);
		}
	}
	else if (arg[0] == DIRECT_CHAR)
		return (check_direct(arg, lbls));
	else if (ft_isdigit(arg[0]))
		return (check_indirect(arg, 2));
	return (0);
}

int		check_args(t_spl *line, t_list *args, char arg_type[3], t_list *lbls)
{
	t_list	*ptr;
	int		arg;
	int		i;

	i = 0;
	ptr = args;
	while (ptr)
	{
		arg = identify_argtype((char*)ptr->content, lbls);
		if (arg == 0 || (arg != ISREG && arg != ISDIR && arg != ISIND))
			print_errors2(2, line->op_code, (char*)ptr->content, line->ln_nb);
		ptr = ptr->next;
		i++;
	}
	return (1);
}