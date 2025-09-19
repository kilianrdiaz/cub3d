/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:28:50 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 13:28:54 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

char		*map[] = {"11111111111111111111", "10000000000000000001",
			"11110111011111111001", "10000001000000001001",
			"10111111101111111001", "100000000N0000000001",
			"10111111111111111111", "11111111111111111111"};

static void	create_spiderman(t_game *g)
{
	t_pos	p;
	char	c;

	p.y = -1;
	while (++p.y < (int)ft_memlen(map))
	{
		p.x = -1;
		while (++p.x < (int)ft_strlen(map[p.y]))
		{
			c = map[p.y][p.x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'P')
			{
                g->spider.posX = p.x + 0.5;
				g->spider.posY = p.y + 0.5;
				set_direction(g, c);
				return ;
			}
		}
	}
	ft_error_exit("Error: No player start position found in map\n");
}
static void load_textures(t_game *g)
{
    load_texture(g, &g->spider.hand, "./textures/spiderhand_01.xpm");
    /* Carga texturas: ajusta paths según tus archivos */
    load_texture(g, &g->floor, "./textures/floor.xpm");
    load_texture(g, &g->ceiling, "./textures/ceiling.xpm");
    load_texture(g, &g->wall_north, "./textures/wall_inner_left.xpm");
    load_texture(g, &g->wall_south, "./textures/wall_inner_right.xpm");
    load_texture(g, &g->wall_east, "./textures/wall_inner_right.xpm");
    load_texture(g, &g->wall_west, "./textures/wall_inner_left.xpm");
}

int key_press(int key, t_game *g)
{
    if (key == 119) g->keys.w = 1;
    if (key == 97)  g->keys.a = 1;
    if (key == 115)  g->keys.s = 1;
    if (key == 100)  g->keys.d = 1;
    if (key == 65307) mlx_destroy_window(g->mlx, g->win);
    return 0;
}

int key_release(int key, t_game *g)
{
    if (key == 119) g->keys.w = 0;
    if (key == 97)  g->keys.a = 0;
    if (key == 115)  g->keys.s = 0;
    if (key == 100)  g->keys.d = 0;
    return 0;
}

int	main(void)
{
    t_game	g;
    
	g.mlx = mlx_init();
	if (!g.mlx)
	{
        ft_printf_fd(STDERR_FILENO, "mlx_init failed\n");
		return (1);
	}
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Cub3D");
	if (!g.win)
	{
        ft_printf_fd(STDERR_FILENO, "mlx_new_window failed\n");
		return (1);
	}
	g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
	if (!g.img)
	{
        ft_printf_fd(STDERR_FILENO, "mlx_new_image failed\n");
		return (1);
	}
	g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_len, &g.endian);
	create_spiderman(&g);
    load_textures(&g);
    ft_bzero(&g.keys, sizeof(t_keys));
    mlx_hook(g.win, 2, 1L<<0, key_press, &g);   // tecla presionada
    mlx_hook(g.win, 3, 1L<<1, key_release, &g); // tecla liberada
    mlx_loop_hook(g.mlx, render, &g);           // loop continuo
	mlx_loop(g.mlx);
	return (0);
}
