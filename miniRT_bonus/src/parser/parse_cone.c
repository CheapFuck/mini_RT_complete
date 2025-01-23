/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cone.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/20 16:21:48 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/23 14:01:52 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

static void	parse_cone_material(t_cone *cone, char **tokens)
{
	cone->material.checker = ft_atoi(tokens[6]);
	cone->material.reflectivity = ft_atof(tokens[7]);
	cone->material.transparency = ft_atof(tokens[8]);
	cone->material.refractive_index = ft_atof(tokens[9]);
}

static void	parse_cone_helper(t_cone *cone)
{
	if (cone->orientation.x == 0 && cone->orientation.y == 0
		&& cone->orientation.z == 0)
	{
		cone->orientation = (t_vector){0, 1, 0};
		printf("Warning: Invalid cone orientation, using default (0, 1, 0)\n");
	}
	else
		cone->orientation = normalize(cone->orientation);
}

void	parse_cone(char *line, t_scene *scene)
{
	char	**tokens;
	t_cone	cone;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 10)
		exit_with_error("Invalid cone format: Expected 10 tokens");
	if (!parse_cone_properties(tokens, &cone))
		return (ft_free_split(tokens));
	parse_cone_helper(&cone);
	cone.diameter = ft_atof(tokens[3]);
	cone.height = ft_atof(tokens[4]);
	cone.radius = cone.diameter / 2.0;
	if (!parse_color(tokens[5], &cone.material.color)
		|| !validate_color(&cone.material.color))
		return (ft_free_split(tokens));
	parse_cone_material(&cone, tokens);
	if (cone.material.reflectivity < 0 || cone.material.reflectivity > 1)
		exit_with_error("Reflectivity must be between 0 and 1");
	if (cone.material.transparency < 0 || cone.material.transparency > 1)
		exit_with_error("Transparency must be between 0 and 1");
	if (cone.material.refractive_index <= 1)
		exit_with_error("Refractive index must be greater than 1");
	if (scene->num_cones >= 100)
		exit_with_error("Error: Maximum number of cones exceeded");
	scene->cones[scene->num_cones++] = cone;
}
