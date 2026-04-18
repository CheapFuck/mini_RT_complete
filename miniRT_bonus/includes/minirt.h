/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minirt.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 16:58:01 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/24 13:17:15 by thivan-d      ########   odam.nl         */
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
# include <pthread.h>
# include <sys/time.h>
# include <sched.h>
# include "scene.h"
# include "objects.h"
# include "render.h"
# include "../MLX42/include/lodepng/lodepng.h"

# define WIDTH  1280
# define HEIGHT 1024
# define EPSILON	1e-6
# define SAMPLES    4
# define MAX_REFLECTION_DEPTH 5
# define CONTRIB_CUTOFF 0.005

#endif // MINIRT_H
