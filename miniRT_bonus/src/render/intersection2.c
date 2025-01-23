/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   intersection2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:11 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:12:29 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

double	length(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

double	distance_from_axis(t_vector p, t_vector axis)
{
	t_vector	projected;

	projected = multiply_scalar(axis, dot(p, axis));
	return (length(subtract(p, projected)));
}

static void	calc_vars_cone(t_intersect_cone *vars, t_cone *cone, t_ray *ray)
{
	double	k;

	vars->oc = subtract(ray->origin, cone->center);
	vars->axis = normalize(cone->orientation);
	vars->dot_dir_axis = dot(ray->direction, vars->axis);
	vars->dot_oc_axis = dot(vars->oc, vars->axis);
	k = (cone->radius / cone->height) * (cone->radius / cone->height);
	vars->d = subtract(ray->direction, multiply_scalar(vars->axis,
				vars->dot_dir_axis));
	vars->o = subtract(vars->oc, multiply_scalar(vars->axis,
				vars->dot_oc_axis));
	vars->a = dot(vars->d, vars->d) - (1 + k) * (vars->dot_dir_axis
			* vars->dot_dir_axis);
	vars->b = 2.0 * (dot(vars->d, vars->o) - (1 + k) * vars->dot_dir_axis
			* vars->dot_oc_axis);
	vars->c = dot(vars->o, vars->o) - (1 + k) * (vars->dot_oc_axis
			* vars->dot_oc_axis);
	vars->discriminant = vars->b * vars->b - 4 * vars->a * vars->c;
}

int	intersect_cone(t_ray *ray, t_cone *cone, double *t)
{
	t_intersect_cone	vars;

	calc_vars_cone(&vars, cone, ray);
	if (vars.discriminant < 0)
		return (0);
	vars.sqrt_discriminant = sqrt(vars.discriminant);
	vars.t1 = (-vars.b - vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.t2 = (-vars.b + vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.p1 = add(ray->origin, multiply_scalar(ray->direction, vars.t1));
	vars.height1 = dot(subtract(vars.p1, cone->center), vars.axis);
	vars.p2 = add(ray->origin, multiply_scalar(ray->direction, vars.t2));
	vars.height2 = dot(subtract(vars.p2, cone->center), vars.axis);
	vars.hit = 0;
	if (vars.t1 > 0 && vars.height1 >= 0 && vars.height1 <= cone->height)
	{
		*t = vars.t1;
		vars.hit = 1;
	}
	if (vars.t2 > 0 && vars.height2 >= 0 && vars.height2 <= cone->height
		&& (!vars.hit || vars.t2 < *t))
	{
		*t = vars.t2;
		vars.hit = 1;
	}
	return (vars.hit);
}
