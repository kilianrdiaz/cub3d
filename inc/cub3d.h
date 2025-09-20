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

# define WIDTH 640
# define HEIGHT 480
# define MAP_W 21
# define MAP_H 8

# define MOVE_SPEED 1
# define ROT_SPEED 1

extern char		*map[];

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
	t_tex		tex;
}				t_sprite;

typedef struct s_keys
{
	int			w;
	int			a;
	int			s;
	int			d;
	int			left;
	int			right;
}				t_keys;

typedef struct s_spidy
{
	double		posX;
	double		posY;
	double		dirX;
	double		dirY;
	double		planeX;
	double		planeY;
	t_tex		hand;
}				t_spidy;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	t_spidy		spider;
	t_tex		floor;
	t_tex		ceiling;
	t_tex		wall_north;
	t_tex		wall_south;
	t_tex		wall_east;
	t_tex		wall_west;
	double zbuffer[WIDTH]; // 👈 guardamos la distancia de cada rayo
	t_sprite	*bombs;
	t_keys		keys;
}				t_game;

typedef struct s_ray
{
	double		cameraX;
	double		dirX0;
	double		dirY0;
	double		dirX1;
	double		dirY1;
	double		posZ;
	double		rowDistance;
	int			mapX;
	int			mapY;
	double		sideDistX;
	double		sideDistY;
	double		deltaDistX;
	double		deltaDistY;
	double		perpWallDist;
	double		stepX;
	double		stepY;
	int hit; /* para evitar warnings */
	int			side;
	int			lineHeight;
	int			drawStart;
	int			drawEnd;
	int			color;
	int			tx;
	int			ty;
}				t_ray;

int				clamp_int(int v, int a, int b);
void			put_pixel(t_game *g, int x, int y, int color);
void			clean_screen(t_game *g);
void			load_texture(t_game *g, t_tex *tex, char *path);
int				render(t_game *g);
int				set_direction(t_game *g, char c);
void			draw_hand(t_game *g);
void			draw_floor_and_ceiling(t_game *g, t_ray *ray, int y);
void			draw_wall_stripe(t_game *g, t_ray *ray, t_tex *tex, int x);
t_tex			*get_texture_wall(t_game *g, t_ray *ray);
void			update_player_position(t_game *g);
void			print_map(t_game *g);
void			init_bombs(t_game *g);
void			draw_sprite(t_game *g, t_sprite *sp);
int				key_press(int key, t_game *g);
int				key_release(int key, t_game *g);

#endif