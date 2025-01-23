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

int	is_in_shadow_sphere(t_in_shadow	*vars, t_scene *scene)
{
	vars->i = 0;
	while (vars->i < scene->num_spheres)
	{
		if (intersect_sphere(&vars->shadow_ray, &scene->spheres[vars->i],
				&vars->t_shadow) && vars->t_shadow < vars->light_distance)
			return (1);
		vars->i++;
	}
	return (0);
}

int	is_in_shadow_cylinder(t_in_shadow *vars, t_scene *scene)
{
	vars->i = 0;
	while (vars->i < scene->num_cylinders)
	{
		if (intersect_cylinder(&vars->shadow_ray, &scene->cylinders[vars->i],
				&vars->t_shadow) && vars->t_shadow < vars->light_distance)
			return (1);
		vars->i++;
	}
	return (0);
}

int	is_in_shadow_plane(t_in_shadow *vars, t_scene *scene)
{
	vars->i = 0;
	while (vars->i < scene->num_planes)
	{
		if (intersect_plane(&vars->shadow_ray, &scene->planes[vars->i],
				&vars->t_shadow) && vars->t_shadow < vars->light_distance)
			return (1);
		vars->i++;
	}
	return (0);
}

int	is_in_shadow_disc(t_in_shadow *vars, t_scene *scene)
{
	vars->i = 0;
	while (vars->i < scene->num_discs)
	{
		if (intersect_disc(&vars->shadow_ray, &scene->discs[vars->i],
				&vars->t_shadow) && vars->t_shadow < vars->light_distance)
			return (1);
		vars->i++;
	}
	return (0);
}

int	is_in_shadow_cone(t_in_shadow *vars, t_scene *scene)
{
	vars->i = 0;
	while (vars->i < scene->num_cones)
	{
		if (intersect_cone(&vars->shadow_ray, &scene->cones[vars->i],
				&vars->t_shadow) && vars->t_shadow < vars->light_distance)
			return (1);
		vars->i++;
	}
	return (0);
}
