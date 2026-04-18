/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render10.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:26 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:37:14 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vector	get_cylinder_normal(t_vector hit_point, t_cylinder *cylinder)
{
	t_vector	axis;
	t_vector	cp;
	t_vector	axis_point;
	t_vector	normal;
	double		dot_prod;

	axis = normalize(cylinder->orientation);
	cp = subtract(hit_point, cylinder->center);
	dot_prod = dot(cp, axis);
	axis_point = add(cylinder->center, multiply_scalar(axis, dot_prod));
	normal = normalize(subtract(hit_point, axis_point));
	return (normal);
}

void	get_hit_normal_plane(t_hit_record *hit, t_vector *normal,
	t_scene *scene, t_ray ray)
{
	t_plane	*plane;

	if (hit->type == PLANE)
	{
		plane = &scene->planes[hit->index];
		*normal = plane->normal;
		if (dot(ray.direction, plane->normal) > 0)
			*normal = multiply_scalar(plane->normal, -1);
		hit->material = plane->material;
	}
}

t_color	apply_refraction(t_material_params params, t_color base_color)
{
	t_ray	refraction_ray;
	t_color	refracted_color;
	double	new_contrib;

	new_contrib = params.contrib * params.hit->material.transparency;
	if (new_contrib < CONTRIB_CUTOFF)
		return (base_color);
	refraction_ray = get_refraction_ray(params.hit->point, params.normal,
			params.ray, params.hit->material.refractive_index);
	refraction_ray.origin = add(refraction_ray.origin,
			multiply_scalar(refraction_ray.direction, 0.001));
	refracted_color = trace_ray(refraction_ray, params.scene,
			params.depth + 1);
	if (dot(params.ray.direction, params.normal) > 0)
		return (refracted_color);
	return (blend_colors(base_color, refracted_color,
			params.hit->material.transparency));
}
