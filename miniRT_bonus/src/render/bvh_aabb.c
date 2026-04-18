/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh_aabb.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/24 16:00:00 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 16:00:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_aabb	aabb_empty(void)
{
	t_aabb	box;

	box.min = (t_vector){INFINITY, INFINITY, INFINITY};
	box.max = (t_vector){-INFINITY, -INFINITY, -INFINITY};
	return (box);
}

t_aabb	aabb_grow(t_aabb box, t_vector p)
{
	box.min.x = fmin(box.min.x, p.x);
	box.min.y = fmin(box.min.y, p.y);
	box.min.z = fmin(box.min.z, p.z);
	box.max.x = fmax(box.max.x, p.x);
	box.max.y = fmax(box.max.y, p.y);
	box.max.z = fmax(box.max.z, p.z);
	return (box);
}

t_aabb	aabb_combine(t_aabb a, t_aabb b)
{
	t_aabb	result;

	result.min.x = fmin(a.min.x, b.min.x);
	result.min.y = fmin(a.min.y, b.min.y);
	result.min.z = fmin(a.min.z, b.min.z);
	result.max.x = fmax(a.max.x, b.max.x);
	result.max.y = fmax(a.max.y, b.max.y);
	result.max.z = fmax(a.max.z, b.max.z);
	return (result);
}

t_aabb	compute_bounds(t_bvh_prim *prims, int start, int end)
{
	t_aabb	box;
	int		i;

	box = aabb_empty();
	i = start;
	while (i < end)
	{
		box = aabb_combine(box, prims[i].bounds);
		i++;
	}
	return (box);
}

int	longest_axis(t_aabb box)
{
	double	dx;
	double	dy;
	double	dz;

	dx = box.max.x - box.min.x;
	dy = box.max.y - box.min.y;
	dz = box.max.z - box.min.z;
	if (dx >= dy && dx >= dz)
		return (0);
	if (dy >= dz)
		return (1);
	return (2);
}
