/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   intersection2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:43 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:44 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static int	solve_quadratic(double *components, double *t1, double *t2)
{
	double	discriminant;

	discriminant = components[1] * components[1] - 4 * components[0]
		* components[2];
	if (discriminant < EPSILON)
		return (0);
	*t1 = (-components[1] - sqrt(discriminant)) / (2.0 * components[0]);
	*t2 = (-components[1] + sqrt(discriminant)) / (2.0 * components[0]);
	return (1);
}

static int	check_cylinder_height(t_ray *ray, t_cylinder *cylinder,
				double t, t_vector axis)
{
	t_vector	intersection;
	double		height;

	intersection = add(ray->origin, multiply_scalar(ray->direction, t));
	height = dot(subtract(intersection, cylinder->center), axis);
	return (fabs(height) <= cylinder->height / 2);
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
	t_vector	axis;
	double		components[3];
	double		t1;
	double		t2;
	int			hit;

	axis = normalize(cylinder->orientation);
	calculate_cylinder_quadratic(ray, cylinder, components);
	if (!solve_quadratic(components, &t1, &t2))
		return (0);
	hit = 0;
	if (t1 > 0 && check_cylinder_height(ray, cylinder, t1, axis))
	{
		*t = t1;
		hit = 1;
	}
	if (t2 > 0 && check_cylinder_height(ray, cylinder, t2, axis) && (!hit || t2
			< *t))
	{
		*t = t2;
		hit = 1;
	}
	return (hit);
}

int	intersect_disc(t_ray *ray, t_disc *disc, double *t)
{
	t_vector	oc;
	double		denom;
	double		t_plane;
	t_vector	p;
	double		distance_squared;

	denom = dot(ray->direction, disc->normal);
	if (fabs(denom) < 1e-6)
		return (0);
	oc = subtract(disc->center, ray->origin);
	t_plane = dot(oc, disc->normal) / denom;
	if (t_plane <= 0)
		return (0);
	p = add(ray->origin, scale(ray->direction, t_plane));
	distance_squared = length_squared(subtract(p, disc->center));
	if (distance_squared > disc->radius * disc->radius)
		return (0);
	*t = t_plane;
	return (1);
}
