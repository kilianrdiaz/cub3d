/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:27:17 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 21:04:09 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libs/libft/libft.h"
# include "../libs/minilibx-linux/mlx.h"
# include <math.h>

# define COLOR_NONE 0xFF000000

# define WIDTH 1920
# define HEIGHT 960
# define GAME_W 1400
# define GAME_HEIGHT 960

# define MINIMAP_PLAYER_SIZE 10
# define VISIBLE_SIZE 300

# define SCALE_SPRITE 2.0

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.1
# define DETECT_RADIUS 7.0

// MINIMAPA
# define COL_BG 0x000000
# define COL_WALL 0xA9A9A9
# define COL_FLOOR 0x000000
# define COL_WALL_BORDER 0x404040
# define COL_UNK_FOG 0x000000
# define COL_PLAYER 0xFF2D2D
# define COL_BOMB 0xFFB84D
# define COL_LIZARD 0x5AFF7A

# define COL_BORDER 0xFFB0B0B0
# define COL_HIGHLIGHT 0xFFFFFFFF
# define COL_FILL 0xFFA00000
# define COL_FILL_MID 0xFF800000
# define COL_FILL_DARK 0xFF500000
# define COL_SHADE 0xFF707070
# define COL_BAR_BG 0xFF202020

# define REVEAL_STEP_RADIUS 3
# define MINIMAP_SIZE_LIMIT 500
# define MINIMAP_OFFSET_X (WIDTH - MINIMAP_SIZE_LIMIT + 80)
# define MINIMAP_OFFSET_Y 200
# define MINIMAP_TILE 20

# define TIMER 125
# define TEXT_DURATION 60.0
# define TEXT_HOLD_TIME 20

typedef struct s_coords
{
	double			x;
	double			y;
}					t_coords;

typedef struct s_timeleft
{
	unsigned int	minutes;
	unsigned int	seconds;
}					t_timeleft;

typedef enum e_state
{
	ACTIVE,
	MOVING,
	ATTACKED,
	ATTACKING,
	NO_RENDER,
	DEFUSED
}					t_state;

typedef enum e_render
{
	INTRO,
	LOAD_LEVEL,
	NEW_LEVEL,
	PLAYING,
	WIN,
	GAME_OVER,
	WAITING_FOR_NAME,
	SCORE_SAVED,
	HIGH_SCORE,
	END
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
	t_coords		pos;
	t_coords		trans;
	double			dist;
	double			inv_det;
	int				width;
	int				height;
	int				screen_x;
	int				found;
	unsigned int	delay;
	double			scale;
	t_state			state;
	t_sprite_type	type;
}					t_sprite;

typedef struct s_spidy
{
	t_coords		pos;
	t_coords		dir;
	t_coords		plane;
	double			move_accum;
	t_tex			*hand;
	t_state			state;
	int				spider_sense;
}					t_spidy;

typedef struct s_lives
{
	int				lives_left;
	t_tex			*spidermask_tex;
	t_sprite		mask_sprite;
	int				player_hp;
}					t_lives;

typedef struct s_minimap
{
	char			**revealed;
	int				width;
	int				height;
	t_pos			offset;
	int				cam_x;
	int				cam_y;
}					t_minimap;

typedef struct s_game
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	t_render		render_state;
	unsigned int	timer;
	int				bomb_count;
	char			**levels;
	unsigned int	level;
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
	t_minimap		minimap;
	unsigned int	score;
	void			*audio;
	void			(*game_loop)(void *);
	t_lives			live;
}					t_game;

typedef struct s_ray
{
	double			view;
	t_coords		coords;
	t_coords		left;
	t_coords		right;
	t_coords		side_dist;
	t_coords		delta_dist;
	double			row_distance;
	int				line_height;
	t_pos			d_start;
	t_pos			d_end;
	t_pos			src;
}					t_ray;

// utils
void				set_error_parsing(t_game *g, char *msg, char *path);
void				load_texture(t_game *g, t_tex *tex, char *path);
int					validate_line(char *line);
int					ft_isspace(int c);
int					check_loaded_textures(t_game *game);
int					clamp_int(int v, int a, int b);
unsigned int		get_pixel_color(t_tex tex, int x, int y);
void				put_pixel(t_game *g, int x, int y, int color);
void				clean_screen(t_game *g);
void				recalc_sprite_scale(t_game *g, t_sprite *sp);
t_tex				get_texture_wall(t_game g, t_ray ray, int dist);
t_sprite			**get_sprites(t_game g);
void				calculate_wall_stripe(t_game g, t_ray *ray, t_tex tex,
						int dist);
void				calculate_distance_to_wall(t_game g, t_ray *ray, int *side);
t_ray				ray_map(t_game g, int x);
char				**get_scores(void);
int					get_position(t_game *g, char **scores);
t_sprite			*print_alphabet(t_game *game);
void				update_scores(char **scores, int position);
int					lizard_on_player(t_game *g);

// parsing
int					check_files_extension(int argc, char **argv);
void				get_info_file(t_game *g);
void				load_map_textures(t_game *g, char **content);
char				**get_map(char **content);
void				create_spiderman(t_game *g);
void				create_sprites(t_game *g);
int					get_map_max_width(char **map);

// rendering
void				game(t_game *g);
int					render(t_game *g);
void				update_bombs(t_game *g);
void				draw_hand(t_game *g, int x);
void				render_sprites(t_game *g);
void				render_char(t_game *g, char c, t_coords coords);
void				render_text(t_game *g, char *str, t_coords coords);
void				load_font(t_game *g, t_font *f, char *path);
void				load_level(t_game *g);
void				show_intro(t_game *g);
void				draw_fullscreen_image(t_game *g, t_tex tex);
void				render_map(t_game *g);
t_timeleft			set_message(t_game *g, char *msg, t_coords coords);
void				timeout_render(t_game *g, t_timeleft t, int render_state);
void				put_timer(t_game *g, t_coords coords);
void				update_timer(t_game *g);
void				render_stats(t_game *g);
void				display_score_panel(t_game *g, t_tex score_panel,
						char **scores);
void					show_high_scores(t_game *g);
void				draw_health_bar(t_game *g);

// minimap rendering
void				draw_minimap(t_game *g);
void				draw_sprites_minimap(t_game *g);
void				draw_player_arrow(t_game *g, t_minimap m);
void				put_rect(t_game *g, t_sprite r, unsigned int color);
void				draw_tile(t_game *g, t_pos tile);
void				init_revealed_if_needed(t_minimap *m, int w, int h);

// input
int					key_press(int key, t_game *g);
int					key_release(int key, t_game *g);

// animation
void				update_player_position(t_game *g);
void				spider_attack(t_game *g);
void				move_lizards(t_game *g);
char				*set_name(t_game *g, t_sprite *alphabet, t_ray ray);

void				free_level(t_game *g);
void				close_program(t_game *g);
void				player_take_damage(t_game *g, t_sprite *lizard);

#endif