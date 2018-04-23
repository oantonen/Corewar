/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header_comment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oantonen <oantonen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/27 18:36:59 by oantonen          #+#    #+#             */
/*   Updated: 2018/04/23 22:06:52 by oantonen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core_asm.h"

void	inspect_str(char *s, char *chars, int *ii)
{
	int		i;
	char	*c;

	i = 0;
	c = NULL;
	while (s[i])
	{
		if (s[i] == '#')
			return ;
		if (!ft_strchr(chars, s[i]))
		{
			c = ft_strsub(&s[i], 0, 1);
			print_errors2(3, "unexpected symbol", c, *ii);
		}
		i++;
	}
}

void	write_cmnt(t_fls *file, char *src, int *ii, int len_d) //без открывающей кавычки
{
	// int		i;

	// i = 0;
	// while (src[i] && src[i] != '"')
	// {
	// 	if (len == COMMENT_LENGTH)
	// 		print_errors(28);
	// 	dst[len] = src[i];
	// 	i++;
	// 	len++;
	// }
	// // ft_printf("srav2=%d\n", src[i]);
	// if (len == COMMENT_LENGTH && src[i] == '\0')
	// 	print_errors(28);
	// if (src[i] == '\0')
	// 	dst[len] = '\n';
	// if (src[i] == '"')
	// {
	// 	file->iscmnt = 1;
	// 	inspect_str(&src[++i], " \t", 1);
	// }

	int		len_s;
	int		i;

	len_s = ft_strlen(src);
	if (ft_strchr(src, '\"'))
		len_s = ft_strchr(src, '\"') - src;
	if (len_s + len_d <= COMMENT_LENGTH && ft_strchr(src, '\"'))
	{
		ft_strncat(file->cmnt, src, len_s);
		file->iscmnt = 1;
		inspect_str(&src[++len_s], " \t", ii);
	}
	else if (len_s + len_d < COMMENT_LENGTH && !ft_strchr(src, '\"'))
	{
		ft_strncat(file->cmnt, src, len_s);
		ft_strcat(file->cmnt, "\n");
	}
	else
	{
		print_errors2(1, "COMMENT", "comment length exceeded, max 2048", *ii);
		// exit(0);
	}
}

void    save_cmnt(t_fls *file, t_list **ptr, char *str, int *i)
{
	char	*c;

	if (!ft_strchr(str, '"'))
		print_errors(26);
	file->cmnt = (char*)ft_memalloc(COMMENT_LENGTH + 1);
	while (*str != '"')
	{
		if (*str == ' ' || *str == '\t')
			str++;
		else
			print_errors(27);
	}
	write_cmnt(file, str + 1, i, (int)ft_strlen(file->cmnt));
	if (file->iscmnt == 0 && !g_is_err)
		*ptr = (*ptr)->next;
	if (file->iscmnt == 0 && !g_is_err)
		while (*ptr && !g_is_err)
		{
			str = (char*)(*ptr)->content;
			if (file->iscmnt == 0)
				write_cmnt(file, str, i, (int)ft_strlen(file->cmnt));
			if (ft_strchr(str, '"'))
				break;
			*ptr = (*ptr)->next;
		}
	ft_printf("%s\n", file->cmnt);
}

void	check_cmnt(t_fls *file, t_list **ptr, int *i)
{
	char	*str;

	if (file->iscmnt)
		print_errors(24);
	str = (char*)(*ptr)->content;
	while (*str && !g_is_err)
	{
		// ft_printf("srav=%d\n", strncmp(str, NAME_CMD_STRING, 5));
		if (*str == ' ' || *str == '\t')
			str++;
		else if (*str == '#')
			return ;
		else if (*str == '.' && !strncmp(str, COMMENT_CMD_STRING, 8))
		{
			// ft_printf("%s\n", str);
			save_cmnt(file, ptr, str + 8, i);
			return ;
		}
		else
			print_errors(25);
	}
}