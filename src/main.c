/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:03:30 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/12/04 23:49:32 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	game_loop(void *param)
{
	t_game	*g;

	g = (t_game *)param;
	if (g->render_state == INTRO)
		show_intro(g);
	else if (g->render_state == LOAD_LEVEL || g->render_state == PRE_LOAD
		|| g->render_state == NEW_LEVEL || g->render_state == GAME_OVER
		|| g->render_state == WIN)
		load_level(g);
	else if (g->render_state == PLAYING || g->render_state == PAUSE)
		game(g);
	else if (g->render_state == HIGH_SCORE
		|| g->render_state == WAITING_FOR_NAME
		|| g->render_state == SCORE_SAVED)
		show_high_scores(g);
	else if (g->render_state == END)
		close_program(g);
}

static void	load_sprite_textures(t_game *g)
{
	g->spider.hand = ft_calloc(sizeof(t_tex), sizeof(t_state));
	if (!g->spider.hand)
		ft_error_exit("Error: Memory allocation failed for spider textures\n");
	load_texture(g, &g->spider.hand[ACTIVE], "./textures/spiderhand_01.xpm");
	load_texture(g, &g->spider.hand[MOVING], "./textures/spiderhand_02.xpm");
	load_texture(g, &g->spider.hand[ATTACKING],
		"./textures/spiderhand_attack.xpm");
	g->bomb_tex = ft_calloc(sizeof(t_tex), sizeof(t_state));
	if (!g->bomb_tex)
		ft_error_exit("Error: Memory allocation failed for bomb textures\n");
	load_texture(g, &g->bomb_tex[ACTIVE], "./textures/bomb.xpm");
	load_texture(g, &g->bomb_tex[ATTACKED], "./textures/bomb_attacked.xpm");
	g->lizard_tex = ft_calloc(sizeof(t_tex), sizeof(t_state));
	if (!g->lizard_tex)
		ft_error_exit("Error: Memory allocation failed for lizard textures\n");
	load_texture(g, &g->lizard_tex[ACTIVE], "./textures/lizard.xpm");
	load_texture(g, &g->lizard_tex[ATTACKED], "./textures/lizard_attacked.xpm");
	load_texture(g, &g->lizard_tex[ATTACKING], "./textures/lizard_attack.xpm");
	load_texture(g, &g->lizard_tex[MOVING], "./textures/lizard_step.xpm");
	g->live.spidermask_tex = ft_calloc(sizeof(t_tex), 2);
	if (!g->live.spidermask_tex)
		ft_error_exit("Error: Memory allocation failed for mask textures\n");
	load_texture(g, &g->live.spidermask_tex[0], "./textures/spidermask.xpm");
	load_texture(g, &g->live.spidermask_tex[1],
		"./textures/spidermask_danger.xpm");
}

static void	prepare_game(t_game *g)
{
	load_font(g, &g->font, "./textures/font.xpm");
	load_sprite_textures(g);
	g->map_text = ft_calloc(sizeof(t_tex), 6);
	if (!g->map_text)
		ft_error_exit("Error: Memory allocation failed for map textures\n");
	g->live.player_hp = 100;
	g->live.lives_left = 3;
	g->wallpaper.color = COLOR_NONE;
	ft_bzero(&g->keys, sizeof(t_keys));
	mlx_hook(g->win, 2, 1L << 0, key_press, g);
	mlx_hook(g->win, 3, 1L << 1, key_release, g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 6, 1L << 6, mouse_rotation, g);
	g->game_loop = game_loop;
}

static void	create_mlx_window(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
		ft_error_exit("mlx_init failed\n");
	g->win = mlx_new_window(g->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!g->win)
		ft_error_exit("mlx_new_window failed\n");
	g->img = mlx_new_image(g->mlx, WIDTH, HEIGHT);
	if (!g->img)
		ft_error_exit("mlx_new_image failed\n");
	g->addr = mlx_get_data_addr(g->img, &g->bpp, &g->line_len, &g->endian);
}

int	main(int argc, char **argv)
{
	t_game	g;

	ft_bzero(&g, sizeof(t_game));
	if (check_files_extension(argc, argv))
		return (1);
	g.levels = argv;
	++g.levels;
	get_info_file(&g, 1);
	--g.levels;
	if (!g.map)
		return (1);
	create_mlx_window(&g);
	prepare_game(&g);
	g.render_state = LOAD_LEVEL;
	mlx_loop_hook(g.mlx, render, &g);
	mlx_loop(g.mlx);
	close_program(&g);
	return (0);
}
