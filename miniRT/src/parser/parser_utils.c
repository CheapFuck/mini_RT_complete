/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:36 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:38 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	validate_ratio(double value, const char *element_name)
{
	if (value < 0.0 || value > 1.0)
	{
		printf("Error: %s ratio must be between 0.0 and 1.0\n", element_name);
		return (0);
	}
	return (1);
}

int	validate_color(t_color *color)
{
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		exit_with_error("Error: Color values must be between 0 and 255");
	return (1);
}

int	validate_nrmlzd_vector(t_vector *vec, char *context)
{
	float	length;

	length = sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
	if (length > 0)
	{
		vec->x /= length;
		vec->y /= length;
		vec->z /= length;
	}
	if (fabs(length - 1.0) > 1e-6)
	{
		printf("%s orientation is not normalized!\n", context);
		return (0);
	}
	return (1);
}

int	validate_fov(int fov)
{
	if (fov < 0 || fov >= 180)
	{
		return (0);
	}
	return (1);
}

int	validate_unique_element(t_scene *scene, char type)
{
	if (type == 'A')
	{
		if (scene->has_ambient)
			exit_with_error("Error: Ambient (A) can only be declared once");
		scene->has_ambient = 1;
	}
	else if (type == 'C')
	{
		if (scene->has_camera)
			exit_with_error("Error: Camera (C) can only be declared once");
		scene->has_camera = 1;
	}
	else if (type == 'L')
	{
		if (scene->has_light)
			exit_with_error("Error: Light (L) can only be declared once");
		scene->has_light = 1;
	}
	return (1);
}
