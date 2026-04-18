/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   bvh_build.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/24 16:00:00 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 16:00:00 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	axis_val(t_vector v, int axis)
{
	if (axis == 0)
		return (v.x);
	if (axis == 1)
		return (v.y);
	return (v.z);
}

static void	make_leaf(t_bvh_node *node, t_aabb bounds, int start, int count)
{
	node->bounds = bounds;
	node->prim_start = start;
	node->prim_count = count;
	node->left = -1;
	node->right = -1;
}

static int	partition_prims(t_bvh_prim *prims, int start,
	int end, t_bvh_split sp)
{
	int			lo;
	int			hi;
	t_bvh_prim	tmp;

	lo = start;
	hi = end - 1;
	while (lo <= hi)
	{
		if (axis_val(prims[lo].centroid, sp.axis) < sp.pos)
			lo++;
		else
		{
			tmp = prims[lo];
			prims[lo] = prims[hi];
			prims[hi] = tmp;
			hi--;
		}
	}
	return (lo);
}

static void	build_node(t_bvh *bvh, int idx, int start, int end)
{
	t_aabb		bounds;
	t_bvh_split	sp;
	int			split;

	bounds = compute_bounds(bvh->prims, start, end);
	if (end - start <= BVH_LEAF_MAX)
	{
		make_leaf(&bvh->nodes[idx], bounds, start, end - start);
		return ;
	}
	sp.axis = longest_axis(bounds);
	sp.pos = (axis_val(bounds.min, sp.axis)
			+ axis_val(bounds.max, sp.axis)) * 0.5;
	split = partition_prims(bvh->prims, start, end, sp);
	if (split == start || split == end)
		split = (start + end) / 2;
	bvh->nodes[idx].bounds = bounds;
	bvh->nodes[idx].prim_count = 0;
	bvh->nodes[idx].left = bvh->num_nodes;
	bvh->nodes[idx].right = bvh->num_nodes + 1;
	bvh->num_nodes += 2;
	build_node(bvh, bvh->nodes[idx].left, start, split);
	build_node(bvh, bvh->nodes[idx].right, split, end);
}

void	build_bvh(t_scene *scene)
{
	int	total;
	int	max_nodes;

	total = scene->num_spheres + scene->num_cylinders
		+ scene->num_cones + scene->num_discs;
	if (total == 0)
		return ;
	max_nodes = 2 * total + 1;
	scene->bvh.nodes = malloc(sizeof(t_bvh_node) * max_nodes);
	scene->bvh.prims = malloc(sizeof(t_bvh_prim) * total);
	if (!scene->bvh.nodes || !scene->bvh.prims)
		exit_with_error("BVH allocation failed");
	scene->bvh.num_nodes = 1;
	scene->bvh.num_prims = 0;
	collect_prims(&scene->bvh, scene);
	build_node(&scene->bvh, 0, 0, scene->bvh.num_prims);
}
