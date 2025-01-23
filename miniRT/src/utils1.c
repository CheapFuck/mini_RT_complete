/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils1.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:09:47 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:09:48 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vector	add(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

t_vector	subtract(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

t_vector	multiply_scalar(t_vector v, double scalar)
{
	t_vector	result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return (result);
}

t_vector	normalize(t_vector v)
{
	double		length;
	t_vector	result;

	if (isnan(v.x) || isnan(v.y) || isnan(v.z))
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
		return (result);
	}
	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length == 0)
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
		return (result);
	}
	result.x = v.x / length;
	result.y = v.y / length;
	result.z = v.z / length;
	return (result);
}
