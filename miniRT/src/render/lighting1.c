/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting1.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:47 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:47 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	combine_color(t_color light_color, t_color object_color)
{
	t_color	result;

	result.r = fmin(255, (light_color.r / 255.0) * object_color.r);
	result.g = fmin(255, (light_color.g / 255.0) * object_color.g);
	result.b = fmin(255, (light_color.b / 255.0) * object_color.b);
	return (result);
}

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

t_ray	generate_shadow_ray(t_vector hit_point, t_light light)
{
	t_ray		shadow_ray;
	t_vector	light_point;
	t_vector	shadow_ray_dir;

	light_point = random_point_on_light(light);
	shadow_ray_dir = normalize(subtract(light_point, hit_point));
	shadow_ray.origin = add(hit_point, multiply_scalar(shadow_ray_dir, 1e-4));
	shadow_ray.direction = shadow_ray_dir;
	return (shadow_ray);
}

double	compute_distance(t_vector a, t_vector b)
{
	t_vector	diff;

	diff = subtract(a, b);
	return (sqrt(dot(diff, diff)));
}

int	intersect_sphere_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_spheres)
	{
		if (intersect_sphere(shadow_ray, &scene->spheres[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}
