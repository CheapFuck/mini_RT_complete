/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render6.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:41 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:37:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	check_plane_intersections(t_ray ray, t_scene *scene, t_hit_record *hit)
{
	int		i;
	double	t_plane;

	t_plane = 0.0;
	i = 0;
	while (i < scene->num_planes)
	{
		if (intersect_plane(&ray, &scene->planes[i], &t_plane)
			&& t_plane < hit->t)
		{
			hit->hit = 1;
			hit->t = t_plane;
			hit->type = PLANE;
			hit->index = i;
		}
		i++;
	}
}

t_color	apply_reflection(t_material_params params, t_color base_color)
{
	t_ray	reflection_ray;
	t_color	reflected_color;
	double	new_contrib;

	new_contrib = params.contrib * params.hit->material.reflectivity;
	if (new_contrib < CONTRIB_CUTOFF)
		return (base_color);
	reflection_ray = get_reflection_ray(params.hit->point,
			params.normal, params.ray);
	reflection_ray.origin = add(reflection_ray.origin,
			multiply_scalar(reflection_ray.direction, 0.001));
	reflected_color = trace_ray(reflection_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, reflected_color,
			params.hit->material.reflectivity));
}
