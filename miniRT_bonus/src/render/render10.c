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

void	check_sphere_intersections(t_ray ray, t_scene *scene,
	t_hit_record *hit)
{
	int		i;
	double	t_sphere;

	t_sphere = 0.0;
	i = 0;
	while (i < scene->num_spheres)
	{
		ray.direction = normalize(ray.direction);
		if (intersect_sphere(&ray, &scene->spheres[i], &t_sphere)
			&& t_sphere < hit->t)
		{
			hit->hit = 1;
			hit->t = t_sphere;
			hit->type = SPHERE;
			hit->index = i;
		}
		i++;
	}
}

t_color	apply_refraction(t_material_params params, t_color base_color)
{
	t_ray	refraction_ray;
	t_color	refracted_color;

	refraction_ray = get_refraction_ray(params.hit->point, params.normal,
			params.ray, params.hit->material.refractive_index);
	refraction_ray.origin = add(refraction_ray.origin,
			multiply_scalar(refraction_ray.direction, 0.001));
	refracted_color = trace_ray(refraction_ray, params.scene,
			params.depth + 1);
	return (blend_colors(base_color, refracted_color,
			params.hit->material.transparency));
}
