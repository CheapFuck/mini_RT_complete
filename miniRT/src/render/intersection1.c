/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   intersection1.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:40 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:42 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
	sphere->oc = subtract(ray->origin, sphere->center);
	sphere->a = dot(ray->direction, ray->direction);
	sphere->b = 2.0 * dot(sphere->oc, ray->direction);
	sphere->c = dot(sphere->oc, sphere->oc) - (sphere->radius * sphere->radius);
	sphere->discriminant = sphere->b * sphere->b - 4 * sphere->a * sphere->c;
	if (sphere->discriminant < 0)
		return (0);
	sphere->sqrt_discriminant = sqrt(sphere->discriminant);
	sphere->t1 = (-sphere->b - sphere->sqrt_discriminant) / (2.0 * sphere->a);
	sphere->t2 = (-sphere->b + sphere->sqrt_discriminant) / (2.0 * sphere->a);
	if (sphere->t1 > 0 && sphere->t2 > 0)
		*t = fmin(sphere->t1, sphere->t2);
	else if (sphere->t1 > 0)
		*t = sphere->t1;
	else if (sphere->t2 > 0)
		*t = sphere->t2;
	else
		return (0);
	return (1);
}

int	intersect_plane(t_ray *ray, t_plane *plane, double *t)
{
	double		denom;
	t_vector	p0_to_origin;
	double		t_temp;

	denom = dot(plane->normal, ray->direction);
	if (fabs(denom) < 1e-6)
		return (0);
	p0_to_origin = subtract(plane->point, ray->origin);
	t_temp = dot(p0_to_origin, plane->normal) / denom;
	if (t_temp < 0)
		return (0);
	*t = t_temp;
	return (1);
}

static void	compute_d_o_vectors(t_ray *ray, t_cylinder *cylinder,
								t_vector *d, t_vector *o)
{
	t_vector	oc;
	t_vector	axis;
	double		dot_dir_axis;
	double		dot_oc_axis;

	oc = subtract(ray->origin, cylinder->center);
	axis = normalize(cylinder->orientation);
	dot_dir_axis = dot(ray->direction, axis);
	dot_oc_axis = dot(oc, axis);
	*d = subtract(ray->direction, multiply_scalar(axis, dot_dir_axis));
	*o = subtract(oc, multiply_scalar(axis, dot_oc_axis));
}

static void	compute_quadratic_components(t_vector *d, t_vector *o,
										double radius, double *components)
{
	components[0] = dot(*d, *d);
	components[1] = 2.0 * dot(*d, *o);
	components[2] = dot(*o, *o) - (radius * radius);
}

void	calculate_cylinder_quadratic(t_ray *ray, t_cylinder *cylinder,
										double *components)
{
	t_vector	d;
	t_vector	o;

	compute_d_o_vectors(ray, cylinder, &d, &o);
	compute_quadratic_components(&d, &o, cylinder->radius, components);
}
