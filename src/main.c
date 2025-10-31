/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:03:30 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/07 11:13:52 by kroyo-di         ###   ########.fr       */
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
}

static void	create_mlx_window(t_game *g)
{
	g->mlx = mlx_init();
	if (!g->mlx)
	{
		ft_printf_fd(STDERR_FILENO, "mlx_init failed\n");
		exit(1);
	}
	g->win = mlx_new_window(g->mlx, WIDTH, HEIGHT, "Cub3D");
	if (!g->win)
	{
		ft_printf_fd(STDERR_FILENO, "mlx_new_window failed\n");
		exit(1);
	}
	g->img = mlx_new_image(g->mlx, WIDTH, HEIGHT);
	if (!g->img)
	{
		ft_printf_fd(STDERR_FILENO, "mlx_new_image failed\n");
		exit(1);
	}
	g->addr = mlx_get_data_addr(g->img, &g->bpp, &g->line_len, &g->endian);
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
	load_font(&g, &g.font, "./textures/font.xpm");
	ft_bzero(&g.keys, sizeof(t_keys));
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);
	mlx_hook(g.win, 3, 1L << 1, key_release, &g);
	mlx_loop_hook(g.mlx, render, &g);
	mlx_loop(g.mlx);
	close_program(&g);
	return (0);
}
