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
    if (!parse_cone_properties(tokens, &cone))
        return (ft_free_split(tokens));
    
    cone.orientation = normalize(cone.orientation); // Ensure normalization
   cone.diameter = ft_atof(tokens[3]); 
    cone.height = ft_atof(tokens[4]);
    // cone.angle = ft_atof(tokens[4]);// * M_PI / 180.0; // Convert to radians
    // if (cone.angle <= 0 ||(cone.angle * M_PI / 180)  >= M_PI / 2)
    //     exit_with_error("Cone angle must be between 0 and π/2 radians");
    
    if (!parse_color(tokens[5], &cone.material.color) || !validate_color(&cone.material.color))
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
	// cone.center = (t_vector){1, 3, 1};  // Cone center at (1, 3, 1)
    // cone.orientation = normalize((t_vector){0, 1, 0});  // Normalized axis (0, 1, 0)
    // cone.radius = 2;  // Radius is half the diameter (4 / 2 = 2)
    // cone.height = 2;  // Height is 2
    // cone.material.color = (t_color){122, 23, 230};  // RGB color (122, 23, 230)
	printf("parsing\n cone.center.x = %f\n cone.center.y = %f\n cone.center.z = %f\n cone.orientation.x = %f\n cone.orientation.y = %f\n cone.orientation.z = %f\n cone.diameter = %f\n cone.height = %f\n cone.radius = %f\n cone.material.color.r = %i\n cone.material.color.g = %i\n cone.material.color.b = %i\n cone.material.transparency = %f\n cone.material.reflectivity = %f\n cone.material.refractive_index = %f\n cone.material.checker = %i\n number of cones = %i\n",
	
	cone.center.x,
	cone.center.y,
	cone.center.z,
	cone.orientation.x,
	cone.orientation.y,
	cone.orientation.z,
	cone.diameter,
	cone.height,
	cone.radius,
	cone.material.color.r,
	cone.material.color.g,
	cone.material.color.b,
	cone.material.transparency,
	cone.material.reflectivity,
	cone.material.refractive_index,
	cone.material.checker,
    scene->num_cones);
	
sleep(1);

    ft_free_split(tokens);
}

