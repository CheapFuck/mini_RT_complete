/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh_traverse.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/24 16:00:00 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 16:00:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

typedef struct s_bvh_isect
{
	t_ray		*ray;
	t_scene		*scene;
	t_vector	inv_dir;
}	t_bvh_isect;

static double	aabb_intersect(t_aabb *box, t_ray *ray, t_vector inv)
{
	double	tmin;
	double	tmax;
	double	t1;
	double	t2;

	t1 = (box->min.x - ray->origin.x) * inv.x;
	t2 = (box->max.x - ray->origin.x) * inv.x;
	tmin = fmin(t1, t2);
	tmax = fmax(t1, t2);
	t1 = (box->min.y - ray->origin.y) * inv.y;
	t2 = (box->max.y - ray->origin.y) * inv.y;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	t1 = (box->min.z - ray->origin.z) * inv.z;
	t2 = (box->max.z - ray->origin.z) * inv.z;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	if (tmax < 0.0 || tmax < tmin)
		return (INFINITY);
	return (fmax(tmin, 0.0));
}

static int	intersect_prim(t_bvh_prim *prim, t_ray *ray,
	double *t, t_scene *scene)
{
	if (prim->type == SPHERE)
		return (intersect_sphere(ray, &scene->spheres[prim->index], t));
	if (prim->type == CYLINDER)
		return (intersect_cylinder(ray, &scene->cylinders[prim->index], t));
	if (prim->type == CONE)
		return (intersect_cone(ray, &scene->cones[prim->index], t));
	if (prim->type == DISC)
		return (intersect_disc(ray, &scene->discs[prim->index], t));
	return (0);
}

static void	test_leaf(t_bvh *bvh, t_bvh_node *node,
	t_bvh_isect *q, t_hit_record *hit)
{
	int		i;
	double	t;

	i = node->prim_start;
	while (i < node->prim_start + node->prim_count)
	{
		t = 0;
		if (intersect_prim(&bvh->prims[i], q->ray, &t, q->scene)
			&& t < hit->t)
		{
			hit->hit = 1;
			hit->t = t;
			hit->type = bvh->prims[i].type;
			hit->index = bvh->prims[i].index;
		}
		i++;
	}
}

int	intersect_bvh_ray(t_bvh *bvh, t_ray *ray,
	t_hit_record *hit, t_scene *scene)
{
	int			stack[BVH_STACK_SIZE];
	t_bvh_isect	q;
	int			sp;
	t_bvh_node	*node;

	if (bvh->num_prims == 0)
		return (0);
	q.ray = ray;
	q.scene = scene;
	q.inv_dir.x = 1.0 / ray->direction.x;
	q.inv_dir.y = 1.0 / ray->direction.y;
	q.inv_dir.z = 1.0 / ray->direction.z;
	sp = 0;
	stack[sp++] = 0;
	while (sp > 0)
	{
		node = &bvh->nodes[stack[--sp]];
		if (aabb_intersect(&node->bounds, ray, q.inv_dir) >= hit->t)
			continue ;
		if (node->prim_count > 0)
			test_leaf(bvh, node, &q, hit);
		else if (sp + 2 <= BVH_STACK_SIZE)
		{
			stack[sp++] = node->left;
			stack[sp++] = node->right;
		}
	}
	return (hit->hit);
}
