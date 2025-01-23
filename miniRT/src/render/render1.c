/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render1.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:58 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:59 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int32_t	ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

double	vector_length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vector	scale(t_vector v, double s)
{
	t_vector	result;

	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return (result);
}

t_ray	create_ray(int x, int y, t_camera *camera)
{
	t_create_ray	vars;

	vars.aspect_ratio = (double)WIDTH / HEIGHT;
	vars.fov_scale = tan((camera->fov * M_PI / 180) / 2);
	vars.norm_x = (2 * (x + 0.5) / WIDTH - 1) * vars.aspect_ratio
		* vars.fov_scale;
	vars.norm_y = (1 - 2 * (y + 0.5) / HEIGHT) * vars.fov_scale;
	vars.right = normalize(cross((t_vector){0, 1, 0}, camera->orientation));
	vars.up = cross(camera->orientation, vars.right);
	vars.ray.origin = camera->pos;
	vars.ray.direction.x = vars.norm_x * vars.right.x + vars.norm_y * vars.up.x
		+ camera->orientation.x;
	vars.ray.direction.y = vars.norm_x * vars.right.y + vars.norm_y * vars.up.y
		+ camera->orientation.y;
	vars.ray.direction.z = vars.norm_x * vars.right.z + vars.norm_y * vars.up.z
		+ camera->orientation.z;
	vars.ray.direction = normalize(vars.ray.direction);
	return (vars.ray);
}

t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}
