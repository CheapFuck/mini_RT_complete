/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render2.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:09:01 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:09:02 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	handle_cylinders(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color)
{
	int			i;
	int			hit;
	double		t_cylinder;
	t_vector	hit_point;
	t_vector	normal;

	hit = 0;
	i = 0;
	while (i < scene->num_cylinders)
	{
		t_cylinder = *t;
		if (intersect_cylinder(ray, &scene->cylinders[i], &t_cylinder)
			&& t_cylinder < *t)
		{
			*t = t_cylinder;
			hit_point = add(ray->origin, multiply_scalar(ray->direction, *t));
			normal = normalize(subtract(hit_point, scene->cylinders[i].center));
			*final_color = apply_lighting(hit_point, normal,
					scene->cylinders[i].color, scene);
			hit = 1;
		}
		i++;
	}
	return (hit);
}

int	handle_single_plane(t_ray *ray, t_plane_params *params)
{
	double		t_plane;
	t_vector	hit_point;
	t_vector	normal;

	t_plane = *params->t;
	if (intersect_plane(ray, params->plane, &t_plane) && t_plane < *params->t)
	{
		*params->t = t_plane;
		hit_point = add(ray->origin, multiply_scalar(ray->direction,
					*params->t));
		normal = params->plane->normal;
		if (dot(ray->direction, normal) > 0)
			normal = multiply_scalar(normal, -1);
		*params->final_color = apply_lighting(hit_point, normal,
				params->plane->color, params->scene);
		return (1);
	}
	return (0);
}

int	handle_planes(t_ray *ray, t_scene *scene, double *t,
		t_color *final_color)
{
	int				i;
	int				hit;
	t_plane_params	params;

	hit = 0;
	i = 0;
	while (i < scene->num_planes)
	{
		params.plane = &scene->planes[i];
		params.t = t;
		params.final_color = final_color;
		params.scene = scene;
		if (handle_single_plane(ray, &params))
			hit = 1;
		i++;
	}
	return (hit);
}

int	handle_single_disc(t_ray *ray, t_disc_params *params)
{
	double		t_disc;
	t_vector	hit_point;
	t_vector	normal;
	t_color		gradient;

	t_disc = *params->t;
	if (intersect_disc(ray, params->disc, &t_disc) && t_disc < *params->t)
	{
		*params->t = t_disc;
		hit_point = add(ray->origin, multiply_scalar(ray->direction,
					*params->t));
		normal = normalize(subtract(hit_point, params->disc->center));
		gradient = apply_lighting(hit_point, normal, params->disc->color,
				params->scene);
		*params->final_color = combine_color(apply_lighting(hit_point,
					params->disc->normal, params->disc->color, params->scene),
				gradient);
		return (1);
	}
	return (0);
}

int	handle_discs(t_ray *ray, t_scene *scene, double *t,
	t_color *final_color)
{
	int				hit;
	int				i;
	t_disc_params	params;

	hit = 0;
	i = 0;
	while (i < scene->num_discs)
	{
		params.disc = &scene->discs[i];
		params.t = t;
		params.final_color = final_color;
		params.scene = scene;
		if (handle_single_disc(ray, &params))
			hit = 1;
		i++;
	}
	return (hit);
}
