/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render5.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:38 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:49:47 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static t_color	get_surface_color_disc(t_scene *scene, t_hit_record *hit)
{
	t_disc	*disc;

	disc = &scene->discs[hit->index];
	if (disc->material.checker == 1)
		return (get_disc_checkerboard_color(hit->point, disc, 0.5));
	else
		return (disc->material.color);
}

static t_color	get_surface_color_cone(t_scene *scene, t_hit_record *hit)
{
	t_cone		*cone;
	t_color		black;
	t_color		white;

	white = (t_color){255, 255, 255};
	black = (t_color){0, 0, 0};
	cone = &scene->cones[hit->index];
	if (cone->material.checker == 1)
	{
		if (is_checkerboard_cone(hit->point, cone, 0.5))
			return (black);
		else
			return (white);
	}
	else
		return (cone->material.color);
}

t_color	get_surface_color(t_hit_record *hit, t_vector normal,
	t_scene *scene, int depth)
{
	t_color	black;
	t_color	object_color;

	scene->dept = depth;
	black = (t_color){0, 0, 0};
	object_color = black;
	if (hit->type == SPHERE)
		object_color = get_surface_color_sphere(scene, hit);
	else if (hit->type == CYLINDER)
		object_color = get_surface_color_cylinder(scene, hit);
	else if (hit->type == PLANE)
		object_color = get_surface_color_plane(scene, hit, normal);
	else if (hit->type == DISC)
		object_color = get_surface_color_disc(scene, hit);
	else if (hit->type == CONE)
		object_color = get_surface_color_cone(scene, hit);
	return (apply_lighting(hit->point, normal, object_color, scene));
}

t_color	calculate_object_color(t_hit_record *hit, t_ray ray,
	t_scene *scene, int depth)
{
	t_vector			normal;
	t_color				final_color;
	t_material_params	params;

	normal = (t_vector){0, 0, 0};
	get_hit_normal(hit, ray, &normal, scene);
	final_color = get_surface_color(hit, normal, scene, depth);
	if (hit->material.reflectivity > 0.0 || hit->material.transparency > 0.0)
	{
		params.hit = hit;
		params.ray = ray;
		params.normal = normal;
		params.scene = scene;
		params.base_color = final_color;
		params.depth = depth;
		final_color = apply_material_effects(params);
	}
	return (final_color);
}

t_color	trace_ray(t_ray ray, t_scene *scene, int depth)
{
	t_hit_record	hit;
	t_color			black;

	black = (t_color){0, 0, 0};
	if (depth > MAX_REFLECTION_DEPTH)
		return (black);
	hit = find_closest_intersection(ray, scene);
	if (hit.hit)
	{
		hit.point = add(ray.origin,
				multiply_scalar(ray.direction, hit.t));
		return (calculate_object_color(&hit, ray, scene, depth));
	}
	return (black);
}
