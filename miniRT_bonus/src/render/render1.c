/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render1.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:26 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:36:22 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_ray	get_reflection_ray(t_vector hit_point, t_vector normal,
	t_ray incident_ray)
{
	t_ray		reflection;
	double		dot_product;
	t_vector	reflection_dir;

	reflection.origin = hit_point;
	dot_product = dot(incident_ray.direction, normal);
	reflection_dir = subtract(incident_ray.direction,
			multiply_scalar(normal, 2.0 * dot_product));
	reflection.direction = normalize(reflection_dir);
	return (reflection);
}

t_color	blend_colors(t_color original_color, t_color reflected_color,
	float reflectivity)
{
	return ((t_color)
		{
			.r = reflectivity * reflected_color.r + (1 - reflectivity)
			* original_color.r,
			.g = reflectivity * reflected_color.g + (1 - reflectivity)
			* original_color.g,
			.b = reflectivity * reflected_color.b + (1 - reflectivity)
			* original_color.b
		});
}

t_vector	get_cone_normal(t_vector hit_point, t_cone *cone)
{
	t_vector	axis;
	t_vector	cp;
	t_vector	axis_point;
	t_vector	normal;
	double		dot_prod;

	axis = normalize(cone->orientation);
	cp = subtract(hit_point, cone->center);
	dot_prod = dot(cp, axis);
	axis_point = add(cone->center, multiply_scalar(axis, dot_prod));
	normal = normalize(subtract(hit_point, axis_point));
	return (normal);
}
