/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/20 12:44:52 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 12:07:44 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vector	normalize(t_vector v)
{
	double		length;
	t_vector	result;

	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length > 0)
	{
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	}
	else
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
	}
	return (result);
}

double	length_squared(t_vector v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

t_vector	scale_vector(t_vector v, double s)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}
