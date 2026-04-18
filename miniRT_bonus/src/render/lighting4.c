/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lighting4.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:56:23 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:48:30 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static double	fast_pow50(double base)
{
	double	p2;
	double	p4;
	double	p8;
	double	p16;
	double	p32;

	if (base <= 0.0)
		return (0.0);
	p2 = base * base;
	p4 = p2 * p2;
	p8 = p4 * p4;
	p16 = p8 * p8;
	p32 = p16 * p16;
	return (p32 * p16 * p2);
}

static int	light_contributes(t_light *light, t_vector hit_point,
	t_vector normal)
{
	t_vector	to_light;
	double		dist_sq;
	double		brightness;

	to_light = subtract(light->pos, hit_point);
	dist_sq = dot(to_light, to_light);
	if (dist_sq < EPSILON)
		return (0);
	brightness = light->brightness / dist_sq;
	if (brightness < 0.001)
		return (0);
	if (dot(to_light, normal) <= 0.0)
		return (0);
	return (1);
}

static void	apply_light_contrib(t_apply_lighting *vars, t_vector hit_point,
	t_vector normal)
{
	vars->light_dir = normalize(subtract(vars->light.pos, hit_point));
	vars->diffuse_intensity = fmax(0.0, dot(normal, vars->light_dir))
		* vars->light.brightness * vars->shadow_factor;
	vars->reflect_dir = normalize(subtract(multiply_scalar(normal, 2.0
					* dot(normal, vars->light_dir)), vars->light_dir));
	vars->specular_intensity = fast_pow50(fmax(0.0,
				dot(vars->reflect_dir,
					vars->view_dir))) * vars->light.brightness
		* vars->shadow_factor * 1;
	vars->light_contribution.r += vars->light.color.r
		* (vars->diffuse_intensity + vars->specular_intensity);
	vars->light_contribution.g += vars->light.color.g
		* (vars->diffuse_intensity + vars->specular_intensity);
	vars->light_contribution.b += vars->light.color.b
		* (vars->diffuse_intensity + vars->specular_intensity);
}

void	apply_lighting_loop(t_apply_lighting *vars, t_scene *scene,
	t_vector hit_point, t_vector normal)
{
	while (vars->i < scene->num_lights)
	{
		vars->light = scene->lights[vars->i];
		if (!light_contributes(&vars->light, hit_point, normal))
		{
			vars->i++;
			continue ;
		}
		vars->shadow_factor = compute_shadow_factor(hit_point, vars->light,
				scene, SAMPLES);
		if (vars->shadow_factor > 0)
			apply_light_contrib(vars, hit_point, normal);
		vars->i++;
	}
}

t_color	apply_lighting(t_vector hit_point, t_vector normal,
	t_color object_color, t_scene *scene)
{
	t_apply_lighting	vars;

	if (scene->dept > MAX_REFLECTION_DEPTH)
		return ((t_color){0, 0, 0});
	vars.light_contribution = (t_color){0, 0, 0};
	vars.light_contribution.r += 255 * scene->ambient.ratio;
	vars.light_contribution.g += 255 * scene->ambient.ratio;
	vars.light_contribution.b += 255 * scene->ambient.ratio;
	vars.view_dir = normalize(subtract(scene->camera.pos, hit_point));
	vars.i = 0;
	apply_lighting_loop(&vars, scene, hit_point, normal);
	return (combine_color(vars.light_contribution, object_color));
}
