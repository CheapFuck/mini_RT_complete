/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   intersection.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:11 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/22 18:23:33 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	intersect_sphere(t_ray *ray, t_sphere *sphere, double *t)
{
	t_vector	oc;
	double		discriminant;
	double		t1;
	double		t2;

	oc = subtract(ray->origin, sphere->center);
	discriminant = pow(dot(oc, ray->direction), 2) - dot(oc, oc)
		+ sphere->radius * sphere->radius;
	if (discriminant < 0)
		return (0);
	discriminant = sqrt(discriminant);
	t1 = (-dot(oc, ray->direction) - discriminant) / dot(ray->direction,
			ray->direction);
	t2 = (-dot(oc, ray->direction) + discriminant) / dot(ray->direction,
			ray->direction);
	if (t1 > 0 && t2 > 0)
		*t = fmin(t1, t2);
	else if (t1 > 0)
		*t = t1;
	else if (t2 > 0)
		*t = t2;
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
	if (fabs(denom) < EPSILON)
		return (0);
	p0_to_origin = subtract(plane->point, ray->origin);
	t_temp = dot(p0_to_origin, plane->normal) / denom;
	if (t_temp < 0)
		return (0);
	*t = t_temp;
	return (1);
}

int	intersect_disc(t_ray *ray, t_disc *disc, double *t)
{
	t_vector	oc;
	double		denom;
	double		t_plane;
	t_vector	p;
	double		distance_squared;

	denom = dot(ray->direction, disc->normal);
	if (fabs(denom) < EPSILON)
		return (0);
	oc = subtract(disc->center, ray->origin);
	t_plane = dot(oc, disc->normal) / denom;
	if (t_plane <= 0)
		return (0);
	p = add(ray->origin, scale_vector(ray->direction, t_plane));
	distance_squared = length_squared(subtract(p, disc->center));
	if (distance_squared > disc->radius * disc->radius)
		return (0);
	*t = t_plane;
	return (1);
}

static void	calc_vars(t_intersect_cylinder *vars, t_cylinder *cylinder,
	t_ray *ray)
{
	vars->oc = subtract(ray->origin, cylinder->center);
	vars->axis = normalize(cylinder->orientation);
	vars->dot_dir_axis = dot(ray->direction, vars->axis);
	vars->dot_oc_axis = dot(vars->oc, vars->axis);
	vars->d = subtract(ray->direction, multiply_scalar(vars->axis,
				vars->dot_dir_axis));
	vars->o = subtract(vars->oc, multiply_scalar(vars->axis,
				vars->dot_oc_axis));
	vars->a = dot(vars->d, vars->d);
	vars->b = 2.0 * dot(vars->d, vars->o);
	vars->c = dot(vars->o, vars->o) - (cylinder->radius * cylinder->radius);
	vars->discriminant = vars->b * vars->b - 4 * vars->a * vars->c;
}

int	intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t)
{
	t_intersect_cylinder	vars;

	calc_vars(&vars, cylinder, ray);
	if (vars.discriminant < 0)
		return (0);
	vars.sqrt_discriminant = sqrt(vars.discriminant);
	vars.t1 = (-vars.b - vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.t2 = (-vars.b + vars.sqrt_discriminant) / (2.0 * vars.a);
	vars.p1 = add(ray->origin, multiply_scalar(ray->direction, vars.t1));
	vars.height1 = dot(subtract(vars.p1, cylinder->center), vars.axis);
	vars.p2 = add(ray->origin, multiply_scalar(ray->direction, vars.t2));
	vars.height2 = dot(subtract(vars.p2, cylinder->center), vars.axis);
	vars.hit = 0;
	if (vars.t1 > 0 && fabs(vars.height1) <= cylinder->height / 2)
	{
		*t = vars.t1;
		vars.hit = 1;
	}
	if (vars.t2 > 0 && fabs(vars.height2) <= cylinder->height / 2
		&& (!vars.hit || vars.t2 < *t))
	{
		*t = vars.t2;
		vars.hit = 1;
	}
	return (vars.hit);
}
double length(t_vector v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
double distance_from_axis(t_vector p, t_vector axis)
{
    // Calculate the perpendicular distance from the point p to the cone's axis
    t_vector projected = multiply_scalar(axis, dot(p, axis));  // Project point p onto axis
    return length(subtract(p, projected));  // Return the distance between the point and its projection on the axis
}
// static void calc_vars_cone(t_intersect_cone *vars, t_cone *cone, t_ray *ray)
// {
//     // Step 1: Calculate vector from ray origin to cone vertex (oc)
//     vars->oc = subtract(ray->origin, cone->center);

//     // Step 2: Normalize the cone's axis
//     vars->axis = normalize(cone->orientation);

//     // Step 3: Compute dot products for later use
//     vars->dot_dir_axis = dot(ray->direction, vars->axis);  // Ray direction vs. cone axis
//     vars->dot_oc_axis = dot(vars->oc, vars->axis);         // oc vector vs. cone axis

//     // Step 4: Compute vectors perpendicular to the cone's axis
//     vars->d = subtract(ray->direction, multiply_scalar(vars->axis, vars->dot_dir_axis));
//     vars->o = subtract(vars->oc, multiply_scalar(vars->axis, vars->dot_oc_axis));

//     // Debugging: Print intermediate variables
//     printf("Vars->d: (%f, %f, %f)\n", vars->d.x, vars->d.y, vars->d.z);
//     printf("Vars->o: (%f, %f, %f)\n", vars->o.x, vars->o.y, vars->o.z);

//     // Step 5: Calculate cone angle properties using radius and height
//     double tan_theta = cone->radius / cone->height;  // Use radius and height to calculate tangent of cone's opening
//     double tan_theta_squared = tan_theta * tan_theta;

//     // Debugging: Print cone properties
//     printf("Cone radius: %f, Cone height: %f, tan_theta: %f\n", cone->radius, cone->height, tan_theta);

//     // Step 6: Compute quadratic equation coefficients
//     vars->a = dot(vars->d, vars->d) - tan_theta_squared * dot(vars->d, vars->axis) * dot(vars->d, vars->axis);
//     vars->b = 2.0 * (dot(vars->d, vars->o) - tan_theta_squared * dot(vars->d, vars->axis) * dot(vars->o, vars->axis));
//     vars->c = dot(vars->o, vars->o) - tan_theta_squared * dot(vars->o, vars->axis) * dot(vars->o, vars->axis);

//     // Debugging: Print coefficients
//     printf("Vars->a: %f, Vars->b: %f, Vars->c: %f\n", vars->a, vars->b, vars->c);

//     // Step 7: Calculate the discriminant for the quadratic equation
//     vars->discriminant = vars->b * vars->b - 4.0 * vars->a * vars->c;

//     // Debugging: Print discriminant
//     printf("Vars->discriminant: %f\n", vars->discriminant);

//     // Step 8: Normalize ray direction (important to ensure consistent results)
//     ray->direction = normalize(ray->direction);

//     // Edge case check: If the ray direction is zero (should not happen in a valid scene)
//     if (fabs(ray->direction.x) < 1e-6 && fabs(ray->direction.y) < 1e-6 && fabs(ray->direction.z) < 1e-6) {
//         printf("Error: Ray direction is zero!\n");
//         return;
//     }
// }
int intersect_cone(t_ray *ray, t_cone *cone, double *t)
{
    t_intersect_cone vars;
    double tan_theta_squared;
    double discriminant;

    // Step 1: Calculate vector from ray origin to cone vertex (oc)
    vars.oc = subtract(ray->origin, cone->center);

    // Step 2: Normalize the cone's axis (assuming it's not already normalized)
    vars.axis = normalize(cone->orientation);

    // Step 3: Compute dot products for ray direction and cone axis
    vars.dot_dir_axis = dot(ray->direction, vars.axis);  // Ray direction vs. cone axis
    vars.dot_oc_axis = dot(vars.oc, vars.axis);         // oc vector vs. cone axis

    // Step 4: Calculate the vector perpendicular to the cone's axis for ray and oc
    vars.d = subtract(ray->direction, multiply_scalar(vars.axis, vars.dot_dir_axis));
    vars.o = subtract(vars.oc, multiply_scalar(vars.axis, vars.dot_oc_axis));

    // Step 5: Compute the tangent squared of the cone's angle (cone's radius squared divided by height squared)
    tan_theta_squared = (cone->radius * cone->radius) / (cone->height * cone->height);

    // Step 6: Calculate the quadratic equation coefficients
    vars.a = dot(vars.d, vars.d) - tan_theta_squared * dot(vars.d, vars.axis) * dot(vars.d, vars.axis);
    vars.b = 2.0 * (dot(vars.d, vars.o) - tan_theta_squared * dot(vars.d, vars.axis) * dot(vars.o, vars.axis));
    vars.c = dot(vars.o, vars.o) - tan_theta_squared * dot(vars.o, vars.axis) * dot(vars.o, vars.axis);

    // Step 7: Calculate the discriminant for the quadratic equation
    discriminant = vars.b * vars.b - 4.0 * vars.a * vars.c;

    // Step 8: If discriminant is negative, no intersection
    if (discriminant < 0) {
        return 0;
    }

    // Step 9: Solve the quadratic equation for t1 and t2 (roots)
    vars.sqrt_discriminant = sqrt(discriminant);
    vars.t1 = (-vars.b - vars.sqrt_discriminant) / (2.0 * vars.a);
    vars.t2 = (-vars.b + vars.sqrt_discriminant) / (2.0 * vars.a);

    // Step 10: Check for valid intersections (t > 0)
    int hit = 0;

    // Check first root (t1)
    if (vars.t1 > 0) {
        vars.p1 = add(ray->origin, multiply_scalar(ray->direction, vars.t1));
        vars.height1 = dot(subtract(vars.p1, cone->center),normalize(vars.axis));

        // If within bounds of the cone's height (relative to the cone's orientation)
        if (vars.height1 >= 0 && vars.height1 <= cone->height) {
            *t = vars.t1;
            hit = 1;
        }
    }

    // Check second root (t2)
    // if (vars.t2 > 0) {
        vars.p2 = add(ray->origin, multiply_scalar(ray->direction, vars.t2));
        vars.height2 = dot(subtract(vars.p2, cone->center), normalize(vars.axis));

        // If within bounds of the cone's height (relative to the cone's orientation)
        if (vars.height2 >= 0 && vars.height2 <= cone->height) {
            *t = vars.t2;
            hit = 1;
        }
    // }

    printf("Discriminant: %f\n", discriminant);
    printf("t1: %f, t2: %f\n", vars.t1, vars.t2);
    printf("Height1: %f, Height2: %f\n", vars.height1, vars.height2);

    // Return whether an intersection was found
    return hit;
}
