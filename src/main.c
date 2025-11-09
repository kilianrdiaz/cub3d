/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:03:30 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 14:56:11 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	load_sprite_textures(t_game *g)
{
	g->spider.hand = ft_calloc(sizeof(t_tex), sizeof(t_state));
	if (!g->spider.hand)
		ft_error_exit("Error: Memory allocation failed for spider textures\n");
	load_texture(g, &g->spider.hand[ACTIVE], "./textures/spiderhand_01.xpm");
	load_texture(g, &g->spider.hand[MOVING], "./textures/spiderhand_02.xpm");
	load_texture(g, &g->spider.hand[ATTACKING], "./textures/spiderhand_attack.xpm");

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

	g->spidermask_tex = ft_calloc(sizeof(t_tex), 2);
	if (!g->lizard_tex)
		ft_error_exit("Error: Memory allocation failed for mask textures\n");
	load_texture(g, &g->lizard_tex[0], "./textures/lizard.xpm");
	
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

void	init_font(t_font *f)
{
	for (int i = 0; i < 128; i++)
	{
		f->chars[i].pixels = NULL;
		f->chars[i].width = 0;
		f->chars[i].height = 0;
	}
}

int	main(int argc, char **argv)
{
	t_game	g;

	ft_bzero(&g, sizeof(t_game));
	create_mlx_window(&g);

	g.map_text = ft_calloc(sizeof(t_tex), 6);
	if (!g.map_text)
		ft_error_exit("Error: Memory allocation failed for map textures\n");

	get_info_file(&g, argc, argv);
	load_sprite_textures(&g);

	init_font(&g.font);
	load_font(&g, &g.font, "./textures/font.xpm");

	g.player_max_hp = 100;
	g.player_hp = 100;
	g.player_last_hit_time = -9999;

	ft_bzero(&g.keys, sizeof(t_keys));
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_loop_hook(g.mlx, render, &g);
	mlx_loop(g.mlx);

	close_program(&g);
	return (0);
}
