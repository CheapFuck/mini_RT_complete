/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils2.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/20 12:44:52 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/22 17:30:25 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

// t_vector	normalize(t_vector v)
// {
// 	double		length;
// 	t_vector	result;

// 	length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
// 	result.x = v.x / length;
// 	result.y = v.y / length;
// 	result.z = v.z / length;
// 	return (result);
// }

t_vector normalize(t_vector v)
{
    double length;
    t_vector result;

    length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    // Check if the vector is non-zero before normalizing
    if (length > 0)
    {
        result.x = v.x / length;
        result.y = v.y / length;
        result.z = v.z / length;
    }
    else
    {
        // Return the zero vector or a default unit vector, depending on your needs
        result.x = 0;
        result.y = 0;
        result.z = 0;
        // Alternatively, you could set it to a unit vector like:
        // result.x = 1; result.y = 0; result.z = 0;
    }

    return result;
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
