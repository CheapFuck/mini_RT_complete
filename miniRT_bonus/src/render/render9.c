/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render9.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:50 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/20 14:27:23 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static int	get_num_threads(void)
{
	int	n;

	n = sysconf(_SC_NPROCESSORS_ONLN);
	if (n < 1)
		n = 4;
	return (n);
}

void	render_scene(mlx_t *mlx, t_scene *scene)
{
	int				i;
	int				num_threads;
	int				rows_per_thread;
	mlx_image_t		*img;
	t_render_data	*data;
	t_thread_data	*thread_data;
	pthread_t		*threads;

	num_threads = get_num_threads();
	rows_per_thread = HEIGHT / num_threads;
	precompute_camera(&scene->camera);
	build_bvh(scene);
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img)
		exit_with_error("Error creating image");
	data = malloc(sizeof(t_render_data));
	if (!data)
		exit_with_error("Error allocating render data");
	init_render_scene(mlx, img, scene, data);
	data->num_threads = num_threads;
	mlx_loop_hook(mlx, update_display, data);
	threads = malloc(sizeof(pthread_t) * num_threads);
	i = 0;
	while (i < num_threads)
	{
		thread_data = malloc(sizeof(t_thread_data));
		thread_data->render_data = data;
		thread_data->thread_id = i;
		thread_data->num_threads = num_threads;
		thread_data->start_row = i * rows_per_thread;
		if (i == num_threads - 1)
			thread_data->end_row = HEIGHT;
		else
			thread_data->end_row = (i + 1) * rows_per_thread;
		pthread_create(&threads[i], NULL, render_thread, thread_data);
		i++;
	}
	free(threads);
	mlx_loop(mlx);
}
