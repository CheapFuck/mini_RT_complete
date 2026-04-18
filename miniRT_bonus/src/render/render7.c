/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render7.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:44 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/20 14:46:38 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_color	apply_material_effects(t_material_params params)
{
	t_color	final_color;

	final_color = params.base_color;
	if (params.hit->material.transparency > 0.0
		&& dot(params.ray.direction, params.normal) > 0)
		return (apply_refraction(params, final_color));
	if (params.hit->material.reflectivity > 0.0)
		final_color = apply_reflection(params, final_color);
	if (params.hit->material.transparency > 0.0)
		final_color = apply_refraction(params, final_color);
	return (final_color);
}

static void	render_thread_loop(t_render_thread *vars)
{
	int		x;
	int		y;
	int		idx;
	uint8_t	*px;

	y = vars->thread_data->start_row;
	while (y < vars->thread_data->end_row)
	{
		x = 0;
		while (x < WIDTH)
		{
			vars->ray = create_ray(x, y, &vars->data->scene->camera);
			vars->final_color = trace_ray(vars->ray, vars->data->scene, 0);
			idx = (y * WIDTH + x) * 4;
			px = &vars->data->img->pixels[idx];
			px[0] = vars->final_color.r;
			px[1] = vars->final_color.g;
			px[2] = vars->final_color.b;
			px[3] = 0xFF;
			x++;
		}
		y++;
	}
}

void	*render_thread(void *arg)
{
	t_render_thread	vars;

	vars = (t_render_thread){0};
	vars.thread_data = (t_thread_data *)arg;
	vars.data = vars.thread_data->render_data;
	render_thread_loop(&vars);
	pthread_mutex_lock(&vars.data->mutex);
	vars.data->threads_completed++;
	if (vars.data->threads_completed == vars.thread_data->num_threads)
	{
		gettimeofday(&vars.data->end_time, NULL);
		vars.elapsed_time = (vars.data->end_time.tv_sec
				- vars.data->start_time.tv_sec) + (vars.data->end_time.tv_usec
				- vars.data->start_time.tv_usec) / 1e6;
		printf("Rendering took %f seconds\n", vars.elapsed_time);
		save_image_to_file(vars.data->img->pixels, WIDTH, HEIGHT, "output.png");
		vars.data->rendering_finished = 1;
	}
	pthread_mutex_unlock(&vars.data->mutex);
	free(vars.thread_data);
	return (NULL);
}
