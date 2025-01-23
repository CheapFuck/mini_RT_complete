/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting5.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:23 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:34:03 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	is_checkerboard_horizontal_cone(t_vector point, t_cone *cone,
	double scale)
{
	t_checkerboard_horizontal	vars;

	vars.grid_size = scale;
	vars.local_point = subtract(point, cone->center);
	vars.height = dot(vars.local_point, cone->orientation);
	vars.radial = subtract(vars.local_point,
			multiply_scalar(cone->orientation, vars.height));
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, cone->orientation)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, cone->orientation));
	vars.y_axis = normalize(cross(cone->orientation, vars.x_axis));
	vars.proj_x = dot(vars.radial, vars.x_axis);
	vars.proj_y = dot(vars.radial, vars.y_axis);
	vars.angle = atan2(vars.proj_y, vars.proj_x);
	vars.normalized_angle = (vars.angle + M_PI) / (2 * M_PI);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.normalized_angle * (2 * M_PI * cone->radius)
			/ vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

int	is_checkerboard_horizontal(t_vector point, t_cylinder *cylinder,
	double scale)
{
	t_checkerboard_horizontal	vars;

	vars.grid_size = scale;
	vars.local_point = subtract(point, cylinder->center);
	vars.height = dot(vars.local_point, cylinder->orientation);
	vars.radial = subtract(vars.local_point,
			multiply_scalar(cylinder->orientation, vars.height));
	vars.up = (t_vector){0, 1, 0};
	if (fabs(dot(vars.up, cylinder->orientation)) > 0.99)
		vars.up = (t_vector){1, 0, 0};
	vars.x_axis = normalize(cross(vars.up, cylinder->orientation));
	vars.y_axis = normalize(cross(cylinder->orientation, vars.x_axis));
	vars.proj_x = dot(vars.radial, vars.x_axis);
	vars.proj_y = dot(vars.radial, vars.y_axis);
	vars.angle = atan2(vars.proj_y, vars.proj_x);
	vars.normalized_angle = (vars.angle + M_PI) / (2 * M_PI);
	vars.u = (int)floor(vars.height / vars.grid_size);
	vars.v = (int)floor(vars.normalized_angle * (2 * M_PI * cylinder->radius)
			/ vars.grid_size);
	return ((vars.u + vars.v) % 2);
}

t_color	get_disc_checkerboard_color(t_vector point, t_disc *disc, double scale)
{
	if (is_disc_checkerboard(point, disc, scale))
		return ((t_color){255, 255, 255});
	return ((t_color){0, 0, 0});
}

int	is_in_shadow(t_vector hit_point, t_light light, t_scene *scene)
{
	t_in_shadow	vars;

	calc_in_shadow_vars(&vars, light, hit_point);
	if (is_in_shadow_sphere(&vars, scene))
		return (1);
	if (is_in_shadow_cylinder(&vars, scene))
		return (1);
	if (is_in_shadow_plane(&vars, scene))
		return (1);
	if (is_in_shadow_disc(&vars, scene))
		return (1);
	if (is_in_shadow_cone(&vars, scene))
		return (1);
	return (0);
}
