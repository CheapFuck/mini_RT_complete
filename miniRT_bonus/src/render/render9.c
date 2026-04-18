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

static void	create_threads(t_render_data *data, int num_threads)
{
	int				i;
	int				rows_per_thread;
	t_thread_data	*td;
	pthread_t		*threads;

	rows_per_thread = HEIGHT / num_threads;
	threads = malloc(sizeof(pthread_t) * num_threads);
	i = 0;
	while (i < num_threads)
	{
		td = malloc(sizeof(t_thread_data));
		td->render_data = data;
		td->thread_id = i;
		td->num_threads = num_threads;
		td->start_row = i * rows_per_thread;
		if (i == num_threads - 1)
			td->end_row = HEIGHT;
		else
			td->end_row = (i + 1) * rows_per_thread;
		pthread_create(&threads[i], NULL, render_thread, td);
		i++;
	}
	free(threads);
}

void	render_scene(mlx_t *mlx, t_scene *scene)
{
	int				num_threads;
	mlx_image_t		*img;
	t_render_data	*data;

	num_threads = get_num_threads();
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
	create_threads(data, num_threads);
	mlx_loop(mlx);
}
