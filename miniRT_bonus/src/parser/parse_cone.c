/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_cone.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/20 16:21:48 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/22 17:59:53 by thivan-d      ########   odam.nl         */
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
void parse_cone(char *line, t_scene *scene)
{
    char    **tokens;
    t_cone  cone;

    tokens = ft_split(line, ' ');
    if (!tokens || ft_arraylen(tokens) != 10)
        exit_with_error("Invalid cone format: Expected 10 tokens");

    // Parsing cone properties
    if (!parse_cone_properties(tokens, &cone))
        return (ft_free_split(tokens));
    
    // Ensure orientation is valid before normalizing
    if (cone.orientation.x == 0 && cone.orientation.y == 0 && cone.orientation.z == 0) {
        cone.orientation = (t_vector){0, 1, 0};  // Default axis if orientation is zero
        printf("Warning: Invalid cone orientation, using default (0, 1, 0)\n");
    } else {
        cone.orientation = normalize(cone.orientation);
    }

    // Parse cone diameter, height, and radius
    cone.diameter = ft_atof(tokens[3]);
    cone.height = ft_atof(tokens[4]);
    cone.radius = cone.diameter / 2.0;  // Correct radius

    // Parse color and material properties
    if (!parse_color(tokens[5], &cone.material.color) || !validate_color(&cone.material.color))
        return (ft_free_split(tokens));
    parse_cone_material(&cone, tokens);

    // Validate material properties
    if (cone.material.reflectivity < 0 || cone.material.reflectivity > 1)
        exit_with_error("Reflectivity must be between 0 and 1");
    if (cone.material.transparency < 0 || cone.material.transparency > 1)
        exit_with_error("Transparency must be between 0 and 1");
    if (cone.material.refractive_index <= 1)
        exit_with_error("Refractive index must be greater than 1");

    // Ensure the number of cones is within the limit
    if (scene->num_cones >= 100)
        exit_with_error("Error: Maximum number of cones exceeded");

    // Add cone to scene
    scene->cones[scene->num_cones++] = cone;

    // Debug output
    printf("Parsing cone properties:\n");
    printf("cone.center.x = %f\n", cone.center.x);
    printf("cone.center.y = %f\n", cone.center.y);
    printf("cone.center.z = %f\n", cone.center.z);
    printf("cone.orientation.x = %f\n", cone.orientation.x);
    printf("cone.orientation.y = %f\n", cone.orientation.y);
    printf("cone.orientation.z = %f\n", cone.orientation.z);
    printf("cone.diameter = %f\n", cone.diameter);
    printf("cone.height = %f\n", cone.height);
    printf("cone.radius = %f\n", cone.radius);  // Corrected radius output
    printf("cone.material.color.r = %i\n", cone.material.color.r);
    printf("cone.material.color.g = %i\n", cone.material.color.g);
    printf("cone.material.color.b = %i\n", cone.material.color.b);
    printf("cone.material.transparency = %f\n", cone.material.transparency);
    printf("cone.material.reflectivity = %f\n", cone.material.reflectivity);
    printf("cone.material.refractive_index = %f\n", cone.material.refractive_index);
    printf("cone.material.checker = %i\n", cone.material.checker);
    printf("Number of cones = %i\n", scene->num_cones);

    ft_free_split(tokens);
}
