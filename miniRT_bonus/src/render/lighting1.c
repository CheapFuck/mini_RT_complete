/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting1.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:17 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:32:46 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vector	random_point_on_light(t_light light)
{
	double		theta;
	double		r;
	t_vector	random_point;

	theta = ((double)rand() / RAND_MAX) * 2 * M_PI;
	r = light.radius * sqrt((double)rand() / RAND_MAX);
	random_point.x = light.pos.x + r * cos(theta);
	random_point.y = light.pos.y + r * sin(theta);
	random_point.z = light.pos.z;
	return (random_point);
}

static void	calc_shadow_vars(t_compute_shadow_factor *vars, t_light light,
	t_vector hit_point)
{
	vars->light_point = random_point_on_light(light);
	vars->shadow_ray_dir = normalize(subtract(vars->light_point, hit_point));
	vars->light_distance = sqrt(dot(subtract(vars->light_point, hit_point),
				subtract(vars->light_point, hit_point)));
	vars->shadow_ray.origin = add(hit_point,
			multiply_scalar(vars->shadow_ray_dir, EPSILON));
	vars->shadow_ray.direction = vars->shadow_ray_dir;
	vars->in_shadow = 0;
	vars->j = 0;
}

double	compute_shadow_factor(t_vector hit_point, t_light light,
	t_scene *scene, int num_samples)
{
	t_compute_shadow_factor	vars;

	vars.unblocked_rays = 0;
	vars.i = 0;
	while (vars.i < num_samples)
	{
		calc_shadow_vars(&vars, light, hit_point);
		vars.in_shadow = is_in_shadow(hit_point, light, scene);
		if (!vars.in_shadow)
			vars.unblocked_rays++;
		vars.i++;
	}
	return ((double)vars.unblocked_rays / num_samples);
}

int	is_checkerboard_cone(t_vector point, t_cone *cone, double scale)
{
	if (fabs(dot((t_vector){0, 1, 0}, cone->orientation)) > 0.99)
		return (is_checkerboard_vertical_cone(point, cone, scale));
	else
		return (is_checkerboard_horizontal_cone(point, cone, scale));
}

int	is_checkerboard(t_vector point, t_cylinder *cylinder, double scale)
{
	if (fabs(dot((t_vector){0, 1, 0}, cylinder->orientation)) > 0.99)
		return (is_checkerboard_vertical(point, cylinder, scale));
	else
		return (is_checkerboard_horizontal(point, cylinder, scale));
}
