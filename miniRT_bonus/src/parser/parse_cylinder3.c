/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cylinder3.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:55:36 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 13:16:23 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

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

	disc = (t_disc){0};
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) < 6)
		return (parse("Error: Invalid disc format for discs", tokens));
	if (!parse_disc_properties(tokens, &disc))
		return (parse("Error: Failed to parse disc properties", tokens));
	disc.diameter = ft_atof(tokens[3]);
	disc.radius = disc.diameter / 2.0;
	disc.height = ft_atof(tokens[4]);
	if (!parse_color(tokens[5], &disc.color))
		return (handle_pars_error(tokens, "Error: Invalid disc color format"));
	disc_parse(scene, disc, tokens);
	ft_free_split(tokens);
}

static void	disc_parse_cone(t_scene *scene, t_disc disc, char **tokens)
{
	t_disc		cone_disc;
	t_vector	offset;

	cone_disc = (t_disc){0};
	offset = multiply_scalar(disc.orientation, disc.height);
	cone_disc.center = add(disc.center, offset);
	cone_disc.normal = disc.orientation;
	cone_disc.radius = disc.radius;
	cone_disc.color = disc.color;
	if (scene->num_discs >= 300)
		return (parse("Error: Maximum number of discs exceeded", tokens));
	scene->discs[scene->num_discs++] = cone_disc;
}

void	parse_disc_cone(char *line, t_scene *scene)
{
	t_disc	disc;
	char	**tokens;
	char	**center_tokens;

	disc = (t_disc){0};
	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) < 6)
		return (parse("Error: Invalid cone format for discs", tokens));
	if (!parse_disc_properties(tokens, &disc))
		return (parse("Error: Failed to parse cone properties", tokens));
	disc.diameter = ft_atof(tokens[3]) * 2;
	disc.radius = disc.diameter / 2.0;
	center_tokens = split_and_validate(tokens[1], 3);
	disc.height = ft_atof(tokens[4]) + (ft_atof(center_tokens[1]));
	if (!parse_color(tokens[5], &disc.color))
		return (handle_pars_error(tokens, "Error: Invalid cone color format"));
	disc_parse_cone(scene, disc, tokens);
	ft_free_split(tokens);
	ft_free_split(center_tokens);
}

int	parse_cylinder_properties(char **tokens, t_cylinder *cylinder)
{
	char	**center_tokens;
	char	**orientation_tokens;

	center_tokens = split_and_validate(tokens[1], 3);
	if (!center_tokens)
		return (0);
	cylinder->center.x = ft_atof(center_tokens[0]);
	cylinder->center.y = ft_atof(center_tokens[1]);
	cylinder->center.z = ft_atof(center_tokens[2]);
	clean_2d_array(center_tokens);
	orientation_tokens = split_and_validate(tokens[2], 3);
	if (!orientation_tokens)
		return (0);
	cylinder->orientation.x = ft_atof(orientation_tokens[0]);
	cylinder->orientation.y = ft_atof(orientation_tokens[1]);
	cylinder->orientation.z = ft_atof(orientation_tokens[2]);
	clean_2d_array(orientation_tokens);
	return (normalize_orientation(cylinder));
}
