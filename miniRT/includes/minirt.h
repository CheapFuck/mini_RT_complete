/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:10:33 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 15:25:46 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/libft.h"
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include "objects.h"
# include "scene.h"
# include "render.h"

# define WIDTH      640
# define HEIGHT     480
# define SAMPLES    8
# define EPSILON	1e-6

#endif // MINIRT_H
