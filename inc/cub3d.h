/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:27:17 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:24:36 by kroyo-di         ###   ########.fr       */
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
# define SCALE_SPRITE 2.0

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.1
# define DETECT_RADIUS 10.0
# define MOVE_SPEED_LIZARD 1

# define LERP_SPEED 1

typedef enum e_state
{
	ACTIVE,
	MOVING,
	ATTACKED,
	ATTACKING,
	DEFUSED
}					t_state;

typedef enum e_render
{
	INTRO,
	PLAYING,
	WIN,
	WAITING_FOR_NAME,
	SCORE_SAVED,
	HIGH_SCORE,
	GAME_OVER
}					t_render;

typedef enum e_map_texture
{
	NO,
	SO,
	EA,
	WE,
	F,
	C
}					t_map_texture;

typedef enum e_sprite_type
{
	BOMB,
	LIZARD
}					t_sprite_type;

typedef struct s_char_bitmap
{
	unsigned int	*pixels;
	int				width;
	int				height;
}					t_char_bitmap;

typedef struct s_font
{
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				char_w;
	int				char_h;
	double			scale;
	t_char_bitmap	chars[128];
}					t_font;

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
	t_render		render_state;
	char			**map;
	t_spidy			spider;
	t_font			font;
	double			zbuffer[WIDTH];
	t_sprite		**bombs;
	t_sprite		**lizards;
	t_tex			*bomb_tex;
	t_tex			*lizard_tex;
	t_tex			*map_text;
	t_keys			keys;
	int				score;
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
int					clamp_int(int v, int a, int b);
void				update_bombs(t_game *g);
void				put_pixel(t_game *g, int x, int y, int color);
void				clean_screen(t_game *g);
void				draw_hand(t_game *g, int x);
t_tex				get_texture_wall(t_game g, t_ray ray);
void				print_map(t_game *g);
void				render_sprites(t_game *g);
t_sprite			**get_sprites(t_game *g);
void				recalc_sprite_scale(t_game *g, t_sprite *sp, double dist);
void				render_text(t_game *g, char *str, t_pos pos);
void				load_font(t_game *g, t_font *f, char *path);
int					show_intro(t_game *g);
void				draw_fullscreen_image(t_game *g, t_tex *tex);
t_ray				ray_map(t_game *g, int x);
void				calculate_wall_stripe(t_game *g, t_ray *ray, t_tex tex);
t_tex				get_texture_wall(t_game g, t_ray ray);
void				render_floor_and_ceiling(t_game *g);
void				render_wall(t_game *g);
char				**get_scores(void);
int					get_position(t_game *g, char **scores);
t_sprite			*print_alphabet(t_game *game, t_tex score_panel);
void				update_scores(char **scores, int position);
void				save_scores(char **scores);
void				display_score_panel(t_game *g, t_tex *score_panel,
						char **scores);
int					show_high_scores(t_game *g);
// input
int					key_press(int key, t_game *g);
int					key_release(int key, t_game *g);

// animation
void				update_player_position(t_game *g);
void				spider_attack(t_game *g);
void				move_lizards(t_game *g);
char				*set_name(t_game *g, t_sprite *alphabet, t_ray ray);
void				close_program(t_game *g);

#endif