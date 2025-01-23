/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:09:50 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/20 12:43:58 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	exit_with_error(const char *msg)
{
	printf("%s\n", msg);
	exit(EXIT_FAILURE);
}

double	dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

double	length_squared(t_vector v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}
