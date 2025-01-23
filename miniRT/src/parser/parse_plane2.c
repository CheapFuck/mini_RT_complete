/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_plane2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:08:30 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 14:08:30 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	parse_plane_tokens(char *token, t_vector *vector)
{
	char	**vector_tokens;

	vector_tokens = split_and_validate(token, 3);
	if (!vector_tokens)
		return (0);
	vector->x = ft_atof(vector_tokens[0]);
	vector->y = ft_atof(vector_tokens[1]);
	vector->z = ft_atof(vector_tokens[2]);
	ft_free_split(vector_tokens);
	return (1);
}
