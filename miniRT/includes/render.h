/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   render.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: thivan-d <thivan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/19 14:10:49 by thivan-d      #+#    #+#                 */
/*   Updated: 2025/01/19 15:44:47 by thivan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "minirt.h"

void			render_scene(mlx_t *mlx, t_scene *scene);
int				intersect_sphere(t_ray *ray, t_sphere *sphere, double *t);
int				intersect_cylinder(t_ray *ray, t_cylinder *cylinder, double *t);
int				intersect_plane(t_ray *ray, t_plane *plane, double *t);
t_color			apply_lighting(t_vector hit_point, t_vector normal,
					t_color object_color, t_scene *scene);
t_color			combine_color(t_color light_color, t_color object_color);
t_vector		random_point_on_light(t_light light);
int				handle_spheres(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color);
t_render_data	*init_render_data(mlx_t *mlx, t_scene *scene, mlx_image_t *img);
void			render_next_row(void *param);
void			calculate_cylinder_quadratic(t_ray *ray, t_cylinder *cylinder,
					double *components);
t_ray			generate_shadow_ray(t_vector hit_point, t_light light);
double			compute_distance(t_vector a, t_vector b);
int				is_in_shadow(t_ray *shadow_ray, t_scene *scene,
					double light_distance);
t_ray			create_ray(int x, int y, t_camera *camera);
t_vector		world_to_local(t_vector point, t_vector orientation,
					t_vector center);
t_vector		scale(t_vector v, double s);
int				intersect_disc(t_ray *ray, t_disc *disc, double *t);
void			parse_discs(char *line, t_scene *scene);
int				parse_disc_properties(char **tokens, t_disc *disc);
t_vector		cross(t_vector a, t_vector b);
void			ft_hook(void *param);
int				intersect_disc_helper(t_scene *scene, t_ray *shadow_ray,
					double t_shadow, double light_distance);
int				intersect_plane_helper(t_scene *scene, t_ray *shadow_ray,
					double t_shadow, double light_distance);
int				intersect_cylinder_helper(t_scene *scene, t_ray *shadow_ray,
					double t_shadow, double light_distance);
void			open_file(const char *filename, int *fd);
int				intersect_sphere_helper(t_scene *scene, t_ray *shadow_ray,
					double t_shadow, double light_distance);
char			*get_next_line_from_file(int fd);
int				parse_plane_tokens(char *token, t_vector *vector);
int				handle_cylinders(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color);
int				handle_single_plane(t_ray *ray, t_plane_params *params);
int				handle_planes(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color);
int				handle_single_disc(t_ray *ray, t_disc_params *params);
int				handle_discs(t_ray *ray, t_scene *scene, double *t,
					t_color *final_color);
t_vector		subtract(t_vector a, t_vector b);
double			dot(t_vector a, t_vector b);
t_vector		normalize(t_vector v);
t_vector		multiply_scalar(t_vector v, double scalar);
t_vector		add(t_vector a, t_vector b);
double			length_squared(t_vector v);
void			parse_file(const char *filename, t_scene *scene);
void			handle_pars_error(char **tokens, const char *error_message);
char			**split_and_validate(char *str, int expected_parts);
int				parse_cylinder_properties(char **tokens, t_cylinder *cylinder);
void			parse_ambient(char *line, t_scene *scene);
void			parse_camera(char *line, t_scene *scene);
void			parse_light(char *line, t_scene *scene);
void			parse_sphere(char *line, t_scene *scene);
void			parse_plane(char *line, t_scene *scene);
void			parse_cylinder(char *line, t_scene *scene);
int				parse_color(char *color_str, t_color *color);
void			exit_with_error(const char *msg);
int				validate_ratio(double value, const char *element_name);
int				validate_color(t_color *color);
int				validate_nrmlzd_vector(t_vector *vec, char *context);
int				validate_fov(int fov);
int				validate_unique_element(t_scene *scene, char type);
int				is_valid_number(const char *str);

#endif // RENDER_H
