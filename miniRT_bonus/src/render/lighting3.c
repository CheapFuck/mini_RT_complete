/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting3.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:23 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:41:03 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	calc_in_shadow_vars(t_in_shadow *vars, t_light light,
	t_vector hit_point)
{
	t_vector	to_light;

	to_light = subtract(light.pos, hit_point);
	vars->light_distance = sqrt(dot(to_light, to_light));
	vars->light_dir = multiply_scalar(to_light, 1.0 / vars->light_distance);
	vars->shadow_ray.origin = add(hit_point, multiply_scalar(vars->light_dir,
				EPSILON));
	vars->shadow_ray.direction = vars->light_dir;
	vars->i = 0;
}
