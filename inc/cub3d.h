/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:27:17 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/12 18:13:03 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "mlx.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>

# define WIDTH 1920
# define HEIGHT 960
# define GAME_WIDTH 1400
# define GAME_HEIGHT 960
# define MAP_W 21
# define MAP_H 8
# define SCALE_SPRITE 2.0

# define MOVE_SPEED 0.07
# define ROT_SPEED 0.05
# define DETECT_RADIUS 10.0
# define MOVE_SPEED_LIZARD 1

extern char		*map[];

typedef enum e_state
{
	ACTIVE,
	MOVING,
	ATTACKED,
	ATTACKING,
	DEFUSED
}	t_state;

typedef struct s_tex
{
	void		*img;
	char		*addr;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
}				t_tex;

typedef struct s_sprite
{
	double		x;
	double		y;
	double		inv_det;
	double		trans_x;
	double		trans_y;
	int			width;
	int			height;
	int			screen_x;
	double		delay;
	double		over_scale;
	t_state		state;
}				t_sprite;

typedef struct s_sprite_order
{
	int			index;
	double		dist;
}				t_sprite_order;

typedef struct s_keys
{
	int			w;
	int			a;
	int			s;
	int			d;
	int			left;
	int			right;
	int			space;
}				t_keys;

typedef struct s_spidy
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
	double		move_accum;
	t_tex		*hand;
	t_state		state;
}				t_spidy;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	int			bomb_count;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	double		timer;
	int			show_intro;
	char		**map;
	t_spidy		spider;
	t_tex		floor;
	t_tex		ceiling;
	t_tex		wall_north;
	t_tex		wall_south;
	t_tex		wall_east;
	t_tex		wall_west;
	double		zbuffer[WIDTH]; // 👈 guardamos la distancia de cada rayo
	t_sprite	**bombs;
	t_sprite    **lizards;
	t_tex		*bomb_tex;
	t_tex		*lizard_tex;
	t_keys		keys;
}				t_game;

typedef struct s_ray
{
	double		camera_x;
	double		dir_x0;
	double		dir_y0;
	double		dir_x1;
	double		dir_y1;
	double		pos_z;
	double		row_distance;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		perp_wall_dist;
	double		step_x;
	double		step_y;
	int			hit; /* para evitar warnings */
	int			side;
	int			line_height;
	int			draw_start_x;
	int			draw_end_x;
	int			draw_start_y;
	int			draw_end_y;
	int			tx;
	int			ty;
	int			color;
}				t_ray;

int				show_intro(t_game *g);
int				clamp_int(int v, int a, int b);
void			update_bombs(t_game *g);
void			put_pixel(t_game *g, int x, int y, int color);
void			clean_screen(t_game *g);
void			load_texture(t_game *g, t_tex *tex, char *path);
int				render(t_game *g);
int				set_direction(t_game *g, char c);
void			spider_attack(t_game *g);
void			draw_hand(t_game *g);
void			draw_floor_and_ceiling(t_game *g, t_ray *ray, int y);
void			draw_wall_stripe(t_game *g, t_ray *ray, t_tex tex, int x);
t_tex			get_texture_wall(t_game g, t_ray ray);
void			update_player_position(t_game *g);
void			print_map(t_game *g);
void			render_sprites(t_game *g, t_sprite **sprites, t_tex *tex);
int				key_press(int key, t_game *g);
int				key_release(int key, t_game *g);
int				close_program(t_game *g);
void			move_lizards(t_game *g);

#endif