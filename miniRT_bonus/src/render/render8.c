/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render8.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:47 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/20 15:58:25 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	precompute_camera(t_camera *camera)
{
	camera->aspect_ratio = (double)WIDTH / HEIGHT;
	camera->fov_scale = tan((camera->fov * M_PI / 180) / 2);
	camera->right = normalize(cross((t_vector){0, 1, 0},
				camera->orientation));
	camera->up = cross(camera->orientation, camera->right);
}

t_ray	create_ray(int x, int y, t_camera *camera)
{
	t_ray	ray;
	double	norm_x;
	double	norm_y;

	norm_x = (2 * (x + 0.5) / WIDTH - 1) * camera->aspect_ratio
		* camera->fov_scale;
	norm_y = (1 - 2 * (y + 0.5) / HEIGHT) * camera->fov_scale;
	ray.origin = camera->pos;
	ray.direction.x = norm_x * camera->right.x + norm_y * camera->up.x
		+ camera->orientation.x;
	ray.direction.y = norm_x * camera->right.y + norm_y * camera->up.y
		+ camera->orientation.y;
	ray.direction.z = norm_x * camera->right.z + norm_y * camera->up.z
		+ camera->orientation.z;
	ray.direction = normalize(ray.direction);
	return (ray);
}

t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

void	update_display(void *param)
{
	t_render_data	*data;

	data = (t_render_data *)param;
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	pthread_mutex_lock(&data->mutex);
	if (data->rendering_finished)
	{
		mlx_terminate(data->mlx);
		pthread_mutex_unlock(&data->mutex);
		pthread_mutex_destroy(&data->mutex);
		free(data);
		exit(0);
	}
	pthread_mutex_unlock(&data->mutex);
}

void	init_render_scene(mlx_t *mlx, mlx_image_t *img, t_scene *scene,
	t_render_data *data)
{
	data->mlx = mlx;
	data->img = img;
	data->scene = scene;
	data->threads_completed = 0;
	data->rendering_finished = 0;
	data->num_threads = 0;
	data->start_time.tv_sec = 0;
	data->start_time.tv_usec = 0;
	data->end_time.tv_sec = 0;
	data->end_time.tv_usec = 0;
	pthread_mutex_init(&data->mutex, NULL);
	gettimeofday(&data->start_time, NULL);
}
