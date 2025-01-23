/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder1.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:08 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:11 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	parse(char *error, char **tokens)
{
	printf("%s\n", error);
	ft_free_split(tokens);
	exit(EXIT_FAILURE);
}

void	parse_cylinder(char *line, t_scene *scene)
{
	t_cylinder	cylinder;
	char		**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 6)
		return (parse("Error: Invalid cylinder format", tokens));
	if (!parse_cylinder_properties(tokens, &cylinder))
		return (ft_free_split(tokens));
	if (!validate_nrmlzd_vector(&cylinder.orientation, "Cylinder orientation"))
		return (ft_free_split(tokens));
	cylinder.diameter = ft_atof(tokens[3]);
	if (cylinder.diameter <= 0)
		return (parse("Error: Cylinder diameter must be positive", tokens));
	cylinder.height = ft_atof(tokens[4]);
	if (cylinder.height <= 0)
		return (parse("Error: Cylinder height must be positive", tokens));
	if (!parse_color(tokens[5], &cylinder.color))
		return (ft_free_split(tokens));
	if (!validate_color(&cylinder.color))
		return (ft_free_split(tokens));
	cylinder.radius = cylinder.diameter / 2.0;
	if (scene->num_cylinders >= 65536)
		return (parse("Error: Maximum number of cylinders exceeded", tokens));
	scene->cylinders[scene->num_cylinders++] = cylinder;
	ft_free_split(tokens);
}

static void	disc_parse(t_scene *scene, t_disc disc, char **tokens)
{
	t_disc		bottom_disc;
	t_vector	bottom_offset;
	t_disc		top_disc;
	t_vector	top_offset;

	bottom_offset = multiply_scalar(disc.orientation, -0.5 * disc.height);
	bottom_disc.center = add(disc.center, bottom_offset);
	bottom_disc.normal = disc.orientation;
	bottom_disc.radius = disc.radius;
	bottom_disc.color = disc.color;
	top_offset = multiply_scalar(disc.orientation, 0.5 * disc.height);
	top_disc.center = add(disc.center, top_offset);
	top_disc.normal = disc.orientation;
	top_disc.radius = disc.radius;
	top_disc.color = disc.color;
	if (scene->num_discs + 2 >= 65535)
		return (parse("Error: Maximum number of discs exceeded", tokens));
	scene->discs[scene->num_discs++] = bottom_disc;
	scene->discs[scene->num_discs++] = top_disc;
}

void	parse_discs(char *line, t_scene *scene)
{
	t_disc	disc;
	char	**tokens;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) < 6)
		return (parse("Error: Invalid cylinder format for discs", tokens));
	if (!parse_disc_properties(tokens, &disc))
		return (parse("Error: Failed to parse cylinder properties", tokens));
	disc.diameter = ft_atof(tokens[3]);
	disc.radius = disc.diameter / 2.0;
	disc.height = ft_atof(tokens[4]);
	if (!parse_color(tokens[5], &disc.color))
		return (handle_pars_error(tokens, "Error: Invalid disc color format"));
	disc_parse(scene, disc, tokens);
	ft_free_split(tokens);
}

int	parse_color(char *color_str, t_color *color)
{
	char	**color_tokens;

	color_tokens = split_and_validate(color_str, 3);
	if (!color_tokens)
		return (0);
	color->r = ft_atoi(color_tokens[0]);
	color->g = ft_atoi(color_tokens[1]);
	color->b = ft_atoi(color_tokens[2]);
	clean_2d_array(color_tokens);
	return (1);
}
