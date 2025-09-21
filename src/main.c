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

char		*map[] = {"11111111111111111111", "100000000000000B0001",
			"11110111011111111001", "10000001000000001001",
			"10111111101111111001", "100000000EBB00000001",
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

/* Llama a esto durante inicialización */
static void	create_bombs(t_game *g)
{
	t_pos		p;
	int			total;
	int			idx;
	t_sprite	*bomb;

	total = MAP_W * MAP_H;
	/* Reservamos un array de punteros, inicializado a NULL */
	g->bombs = malloc(sizeof(t_sprite *) * total);
	if (!g->bombs)
		ft_error_exit("Error: Memory allocation failed for bombs array\n");
	ft_bzero(g->bombs, sizeof(t_sprite *) * total);
	p.y = -1;
	while (++p.y < MAP_H)
	{
		p.x = -1;
		while (++p.x < MAP_W)
		{
			if (map[p.y][p.x] == 'B')
			{
				idx = p.x + p.y * MAP_W;
				bomb = malloc(sizeof(t_sprite));
				if (!bomb)
					ft_error_exit("Error: Memory allocation failed for bomb\n");
				ft_bzero(bomb, sizeof(t_sprite));
				bomb->x = p.x;
				bomb->y = p.y;
				load_texture(g, &bomb->tex, "./textures/bomb.xpm");
				g->bombs[idx] = bomb;
			}
		}
	}
}

static void	load_textures(t_game *g)
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
	create_bombs(&g);
	load_textures(&g);
	ft_bzero(&g.keys, sizeof(t_keys));
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);   // tecla presionada
	mlx_hook(g.win, 3, 1L << 1, key_release, &g); // tecla liberada
	mlx_loop_hook(g.mlx, render, &g);             // loop continuo
	mlx_loop(g.mlx);
	return (0);
}
