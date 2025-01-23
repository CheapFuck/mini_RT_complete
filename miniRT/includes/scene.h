/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   scene.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:10:53 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/20 15:28:24 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

typedef struct s_camera
{
	t_vector	pos;
	t_vector	orientation;
	double		fov;
}	t_camera;

typedef struct s_light
{
	t_vector	pos;
	double		brightness;
	t_color		color;
	double		radius;
}	t_light;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_vector	center;
	t_vector	orientation;
	double		diameter;
	double		height;
	double		radius;
	t_color		color;
}	t_cylinder;

typedef struct s_disc
{
	double		diameter;
	double		height;
	t_vector	orientation;
	t_vector	center;
	t_vector	normal;
	double		radius;
	t_color		color;
}	t_disc;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	camera;
	t_light		lights[100];
	t_plane		planes[100];
	t_sphere	spheres[100];
	t_cylinder	cylinders[100];
	t_disc		discs[200];
	int			num_spheres;
	int			num_planes;
	int			num_cylinders;
	int			num_discs;
	int			num_lights;
	int			has_ambient;
	int			has_camera;
	int			has_light;
}	t_scene;

typedef struct s_disc_params
{
	t_disc	*disc;
	double	*t;
	t_color	*final_color;
	t_scene	*scene;
}	t_disc_params;

typedef struct s_plane_params
{
	t_plane	*plane;
	double	*t;
	t_color	*final_color;
	t_scene	*scene;
}	t_plane_params;

typedef struct s_render_data
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_scene		*scene;
	int			current_row;
	bool		render_complete;
}	t_render_data;

typedef struct s_create_ray
{
	t_ray		ray;
	double		aspect_ratio;
	double		fov_scale;
	double		norm_x;
	double		norm_y;
	t_vector	right;
	t_vector	up;
}	t_create_ray;

#endif // SCENE_H
