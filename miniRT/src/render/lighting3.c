/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting3.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:53 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:55 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	intersect_cylinder_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_cylinder(shadow_ray, &scene->cylinders[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

int	intersect_plane_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_cylinders)
	{
		if (intersect_plane(shadow_ray, &scene->planes[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

int	intersect_disc_helper(t_scene *scene, t_ray *shadow_ray,
	double t_shadow, double light_distance)
{
	int	i;

	i = 0;
	while (i < scene->num_discs)
	{
		if (intersect_disc(shadow_ray, &scene->discs[i], &t_shadow)
			&& t_shadow < light_distance)
			return (1);
		i++;
	}
	return (0);
}

int	is_in_shadow(t_ray *shadow_ray, t_scene *scene,
	double light_distance)
{
	double		t_shadow;

	t_shadow = 0.0;
	if (intersect_sphere_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_cylinder_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_plane_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	if (intersect_disc_helper(scene, shadow_ray, t_shadow, light_distance))
		return (1);
	return (0);
}
