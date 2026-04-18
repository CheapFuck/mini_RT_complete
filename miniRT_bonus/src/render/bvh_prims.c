/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh_prims.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/24 16:00:00 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 16:00:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	add_sphere(t_bvh *bvh, t_sphere *s, int idx)
{
	t_bvh_prim	*p;
	t_vector	r;

	p = &bvh->prims[bvh->num_prims++];
	r = (t_vector){s->radius, s->radius, s->radius};
	p->type = SPHERE;
	p->index = idx;
	p->bounds.min = subtract(s->center, r);
	p->bounds.max = add(s->center, r);
	p->centroid = s->center;
}

static void	add_cylinder(t_bvh *bvh, t_cylinder *c, int idx)
{
	t_bvh_prim	*p;
	t_vector	axis;
	t_vector	half;
	t_vector	ext;

	p = &bvh->prims[bvh->num_prims++];
	p->type = CYLINDER;
	p->index = idx;
	p->centroid = c->center;
	axis = normalize(c->orientation);
	half = multiply_scalar(axis, c->height / 2.0);
	ext.x = c->radius * sqrt(fmax(0, 1.0 - axis.x * axis.x));
	ext.y = c->radius * sqrt(fmax(0, 1.0 - axis.y * axis.y));
	ext.z = c->radius * sqrt(fmax(0, 1.0 - axis.z * axis.z));
	p->bounds = aabb_empty();
	p->bounds = aabb_grow(p->bounds, add(add(c->center, half), ext));
	p->bounds = aabb_grow(p->bounds, subtract(add(c->center, half), ext));
	p->bounds = aabb_grow(p->bounds, add(subtract(c->center, half), ext));
	p->bounds = aabb_grow(p->bounds, subtract(subtract(c->center, half), ext));
}

static void	add_cone(t_bvh *bvh, t_cone *c, int idx)
{
	t_bvh_prim	*p;
	t_vector	axis;
	t_vector	apex;
	t_vector	ext;

	p = &bvh->prims[bvh->num_prims++];
	p->type = CONE;
	p->index = idx;
	axis = normalize(c->orientation);
	p->centroid = add(c->center,
			multiply_scalar(axis, c->height * 0.5));
	apex = add(c->center, multiply_scalar(axis, c->height));
	ext.x = c->radius * sqrt(fmax(0, 1.0 - axis.x * axis.x));
	ext.y = c->radius * sqrt(fmax(0, 1.0 - axis.y * axis.y));
	ext.z = c->radius * sqrt(fmax(0, 1.0 - axis.z * axis.z));
	p->bounds = aabb_empty();
	p->bounds = aabb_grow(p->bounds, add(c->center, ext));
	p->bounds = aabb_grow(p->bounds, subtract(c->center, ext));
	p->bounds = aabb_grow(p->bounds, apex);
}

static void	add_disc(t_bvh *bvh, t_disc *d, int idx)
{
	t_bvh_prim	*p;
	t_vector	ext;
	t_vector	pad;

	p = &bvh->prims[bvh->num_prims++];
	p->type = DISC;
	p->index = idx;
	p->centroid = d->center;
	ext.x = d->radius * sqrt(fmax(0, 1.0 - d->normal.x * d->normal.x));
	ext.y = d->radius * sqrt(fmax(0, 1.0 - d->normal.y * d->normal.y));
	ext.z = d->radius * sqrt(fmax(0, 1.0 - d->normal.z * d->normal.z));
	pad = (t_vector){0.01, 0.01, 0.01};
	p->bounds = aabb_empty();
	p->bounds = aabb_grow(p->bounds, add(add(d->center, ext), pad));
	p->bounds = aabb_grow(p->bounds, subtract(subtract(d->center, ext), pad));
}

void	collect_prims(t_bvh *bvh, t_scene *scene)
{
	int	i;

	bvh->num_prims = 0;
	i = -1;
	while (++i < scene->num_spheres)
		add_sphere(bvh, &scene->spheres[i], i);
	i = -1;
	while (++i < scene->num_cylinders)
		add_cylinder(bvh, &scene->cylinders[i], i);
	i = -1;
	while (++i < scene->num_cones)
		add_cone(bvh, &scene->cones[i], i);
	i = -1;
	while (++i < scene->num_discs)
		add_disc(bvh, &scene->discs[i], i);
}
