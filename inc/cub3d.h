/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:27:17 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/26 18:33:42 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <math.h>

# define COLOR_NONE 0xFF000000

# define WIDTH 1920
# define HEIGHT 960
# define GAME_WIDTH 1400
# define GAME_HEIGHT 960

# define MINIMAP_PLAYER_SIZE 10    // 🔹 radio del punto del jugador

# define SCALE_SPRITE 2.0

# define MOVE_SPEED 0.07
# define ROT_SPEED 0.05
# define DETECT_RADIUS 10.0
# define MOVE_SPEED_LIZARD 1

# define LERP_SPEED 1

// MINIMAPA
# define COL_BG             0x000000
# define COL_WALL           0xA9A9A9
# define COL_FLOOR          0x000000
# define COL_WALL_BORDER    0x404040
# define COL_UNK_FOG        0x000000
# define COL_PLAYER         0xFF2D2D
# define COL_BOMB           0xFFB84D
# define COL_LIZARD         0x5AFF7A

# define INIT_REVEAL_RADIUS 3
# define REVEAL_STEP_RADIUS 1
# define MINIMAP_SIZE_LIMIT 500
# define MINIMAP_OFFSET_X   (WIDTH - MINIMAP_SIZE_LIMIT - 20)
# define MINIMAP_OFFSET_Y   250

extern char			*map[];

typedef enum e_state
{
	ACTIVE,
	MOVING,
	ATTACKED,
	ATTACKING,
	DEFUSED
}					t_state;

typedef enum e_sprite_type
{
	BOMB,
	LIZARD
}					t_sprite_type;

typedef struct s_tex
{
	void			*img;
	char			*addr;
	int				width;
	int				height;
	int				bpp;
	int				line_len;
	int				endian;
	unsigned int	color;
}					t_tex;

typedef struct s_sprite
{
	double			x;
	double			y;
	double			inv_det;
	double			trans_x;
	double			trans_y;
	int				width;
	int				height;
	int				screen_x;
	double			delay;
	double			scale;
	t_state			state;
	t_sprite_type	type;
}					t_sprite;

typedef struct s_sprite_order
{
	int				index;
	double			dist;
}					t_sprite_order;

typedef struct s_keys
{
	int				w;
	int				a;
	int				s;
	int				d;
	int				left;
	int				right;
	int				space;
}					t_keys;

typedef struct s_spidy
{
	double			x;
	double			y;
	double			dir_x;
	double			dir_y;
	double			plane_x;
	double			plane_y;
	double			move_accum;
	t_tex			*hand;
	t_state			state;
}					t_spidy;

typedef struct s_sprite_info
{
	int		t;
	int		ox;
	int		oy;
	int		color;
}	t_sprite_info;

typedef struct s_triangle
{
	int		x[3];
	int		y[3];
	int		color;
}	t_triangle;

typedef struct s_map
{
	char			**map;
	int				height;
	int				width;
}					t_map;

typedef struct	s_minimap
{
	char	**revealed;
	int		width;
	int		height;
}				t_minimap;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	int				bomb_count;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	double			timer;
	int				show_intro;
	char			**map;
	t_spidy			spider;
	t_tex			floor;
	t_tex			ceiling;
	t_tex			wall_north;
	t_tex			wall_south;
	t_tex			wall_east;
	t_tex			wall_west;
	double			zbuffer[WIDTH];
	t_sprite		**bombs;
	t_sprite		**lizards;
	t_tex			*bomb_tex;
	t_tex			*lizard_tex;
	t_keys			keys;
	t_minimap		minimap;
}					t_game;

typedef struct s_ray
{
	double			camera_x;
	double			dir_x0;
	double			dir_y0;
	double			dir_x1;
	double			dir_y1;
	double			pos_z;
	double			row_distance;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	double			perp_wall_dist;
	double			step_x;
	double			step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start_x;
	int				draw_end_x;
	int				draw_start_y;
	int				draw_end_y;
	int				tx;
	int				ty;
	int				color;
}					t_ray;



// parsing

void				get_info_file(t_game *g, int argc, char **argv);
void				load_texture(t_game *g, t_tex *tex, char *path);
void				load_map_textures(t_game *g, char **content);
char				**get_map(char **content);
void				create_spiderman(t_game *g);
void				create_sprites(t_game *g);
int					check_loaded_textures(t_game *game);
int					validate_line(char *line);
int					ft_isspace(int c);

// rendering

int					render(t_game *g);
int					show_intro(t_game *g);
void				draw_floor_and_ceiling(t_game *g, t_ray *ray, int y);
void				draw_wall_stripe(t_game *g, t_ray *ray, t_tex tex, int x);
int					clamp_int(int v, int a, int b);
void				update_bombs(t_game *g);
void				put_pixel(t_game *g, int x, int y, int color);
void				clean_screen(t_game *g);
void				draw_hand(t_game *g);
t_tex				get_texture_wall(t_game g, t_ray ray);
void				print_map(t_game *g);
void				render_sprites(t_game *g);
t_sprite			**get_sprites(t_game *g);
void				recalc_sprite_scale(t_game *g, t_sprite *sp, double dist);

// minimap rendering
void				draw_minimap(t_game *g);
void				draw_map_tiles(t_game *g, t_minimap *m, int tile, int ox, int oy);
void				draw_sprites_minimap(t_game *g, t_minimap *m, t_sprite **arr, t_sprite_info *inf);
void				draw_player_arrow(t_game *g, int tile, int ox, int oy);
void				init_revealed_if_needed(t_minimap *m, int w, int h);
void				reveal_radius(t_minimap *m, int cx, int cy, int r);
void				update_revealed(t_minimap *m, int px, int py);
void				put_rect(t_game *g, int x, int y, int w, int h, int c);

// input
int					key_press(int key, t_game *g);
int					key_release(int key, t_game *g);

// moves
void				update_player_position(t_game *g);
void				spider_attack(t_game *g);
void				move_lizards(t_game *g);

int					close_program(t_game *g);

#endif