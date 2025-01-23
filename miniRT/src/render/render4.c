/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render4.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:09:08 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:09:09 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static uint32_t	trace_ray(t_ray ray, t_scene *scene)
{
	double		t;
	t_color		final_color;
	int			hit;

	t = INFINITY;
	hit = 0;
	final_color = (t_color){0, 0, 0};
	if (handle_spheres(&ray, scene, &t, &final_color))
		hit = 1;
	if (handle_cylinders(&ray, scene, &t, &final_color))
		hit = 1;
	if (handle_planes(&ray, scene, &t, &final_color))
		hit = 1;
	if (handle_discs(&ray, scene, &t, &final_color))
		hit = 1;
	if (hit)
		return ((final_color.r << 24) | (final_color.g << 16)
			| (final_color.b << 8) | 0xFF);
	return (0x000000FF);
}

void	render_next_row(void *param)
{
	t_render_data	*data;
	int				x;
	t_ray			ray;
	uint32_t		color;

	data = (t_render_data *)param;
	if (data->current_row >= HEIGHT)
	{
		data->render_complete = true;
		return ;
	}
	x = 0;
	while (x < WIDTH)
	{
		ray = create_ray(x, data->current_row, &data->scene->camera);
		color = trace_ray(ray, data->scene);
		mlx_put_pixel(data->img, x, data->current_row, color);
		x++;
	}
	data->current_row++;
}
