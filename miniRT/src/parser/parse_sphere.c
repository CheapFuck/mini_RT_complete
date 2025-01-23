/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_sphere.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:33 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:34 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static void	sphere_parse(char **tokens, char **pos, t_sphere sphere,
		t_scene *scene)
{
	char	**colors;

	colors = ft_split(tokens[3], ',');
	if (!colors || ft_arraylen(colors) != 3)
	{
		ft_free_split(tokens);
		ft_free_split(pos);
		exit_with_error("Invalid sphere color format");
	}
	sphere.color.r = ft_atoi(colors[0]);
	sphere.color.g = ft_atoi(colors[1]);
	sphere.color.b = ft_atoi(colors[2]);
	if (!validate_color(&sphere.color))
	{
		ft_free_split(tokens);
		ft_free_split(pos);
		ft_free_split(colors);
		return ;
	}
	scene->spheres[scene->num_spheres] = sphere;
	scene->num_spheres++;
	ft_free_split(colors);
	ft_free_split(tokens);
	ft_free_split(pos);
}

void	parse_sphere(char *line, t_scene *scene)
{
	static t_sphere	sphere;
	char			**tokens;
	char			**pos;

	tokens = ft_split(line, ' ');
	if (!tokens || ft_arraylen(tokens) != 4)
		exit_with_error("Invalid sphere format");
	pos = ft_split(tokens[1], ',');
	if (!pos || ft_arraylen(pos) != 3)
	{
		ft_free_split(tokens);
		exit_with_error("Invalid sphere position format");
	}
	sphere.center.x = ft_atof(pos[0]);
	sphere.center.y = ft_atof(pos[1]);
	sphere.center.z = ft_atof(pos[2]);
	sphere.radius = ft_atof(tokens[2]) / 2.0;
	if (sphere.radius <= 0)
	{
		printf("Error: Sphere diameter must be positive\n");
		ft_free_split(tokens);
		ft_free_split(pos);
		exit(EXIT_FAILURE);
	}
	sphere_parse(tokens, pos, sphere, scene);
}

int	is_valid_number(const char *str)
{
	int	i;
	int	has_digit;
	int	decimal_points;

	i = 0;
	has_digit = 0;
	decimal_points = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			has_digit = 1;
		else if (str[i] == '.')
		{
			decimal_points++;
			if (decimal_points > 1)
				return (0);
		}
		else
			return (0);
		i++;
	}
	return (i > 0 && has_digit);
}
