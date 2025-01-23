/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_arraylen.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:11:48 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:11:49 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_arraylen(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}
