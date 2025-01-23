/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render3.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:09:04 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:09:05 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static mlx_image_t	*create_image(mlx_t *mlx)
{
	mlx_image_t	*img;

	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
		exit_with_error("Error creating image");
	return (img);
}

void	render_scene(mlx_t *mlx, t_scene *scene)
{
	mlx_image_t		*img;
	t_render_data	*data;

	img = create_image(mlx);
	data = init_render_data(mlx, scene, img);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop_hook(mlx, ft_hook, data);
	mlx_loop_hook(mlx, render_next_row, data);
	mlx_loop(mlx);
	free(data);
}

t_vector	world_to_local(t_vector point, t_vector orientation,
	t_vector center)
{
	t_vector	local_point;
	t_vector	up;
	t_vector	right;
	t_vector	forward;
	t_vector	temp_vector;

	local_point = subtract(point, center);
	up = orientation;
	if (fabs(up.y) < 0.999)
		temp_vector = (t_vector){0, 1, 0};
	else
		temp_vector = (t_vector){1, 0, 0};
	right = normalize(cross(up, temp_vector));
	if (fabs(up.y) < 0.999)
		temp_vector = (t_vector){0, 1, 0};
	else
		temp_vector = (t_vector){0, 1, 0};
	right = normalize(cross(up, temp_vector));
	forward = cross(right, up);
	return ((t_vector)
		{
			dot(local_point, right),
			dot(local_point, up),
			dot(local_point, forward)
		});
}

int	handle_spheres(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color)
{
	int			i;
	int			hit;
	double		t_sphere;
	t_vector	hit_point;
	t_vector	normal;

	hit = 0;
	i = 0;
	while (i < scene->num_spheres)
	{
		t_sphere = *t;
		if (intersect_sphere(ray, &scene->spheres[i], &t_sphere)
			&& t_sphere < *t)
		{
			*t = t_sphere;
			hit_point = add(ray->origin, multiply_scalar(ray->direction, *t));
			normal = normalize(subtract(hit_point, scene->spheres[i].center));
			*final_color = apply_lighting(hit_point, normal,
					scene->spheres[i].color, scene);
			hit = 1;
		}
		i++;
	}
	return (hit);
}

t_render_data	*init_render_data(mlx_t *mlx, t_scene *scene,
										mlx_image_t *img)
{
	t_render_data	*data;

	data = malloc(sizeof(t_render_data));
	if (!data)
		exit_with_error("Failed to allocate render data");
	data->mlx = mlx;
	data->img = img;
	data->scene = scene;
	data->current_row = 0;
	data->render_complete = false;
	return (data);
}
