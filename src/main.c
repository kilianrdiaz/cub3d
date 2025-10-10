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

char		*map[] = {"11111111111111111111", "100000000000000B0001",
			"11110111011111111001", "10000001000000001001",
			"10111111101111111001", "100000000EBB0000L001",
			"10111111111111111111", "11111111111111111111"};
char        map_str[] = "11111111111111111111\n100000000000000B0001\n11110111011111111001\n10000001000000001001\n10111111101111111001\n100000000EBB0000L001\n10111111111111111111\n11111111111111111111\n";

static void	create_spiderman(t_game *g)
{
	t_pos	p;
	char	c;

	p.y = -1;
	while (g->map[++p.y])
	{
		p.x = -1;
		while (g->map[p.y][++p.x])
		{
			c = map[p.y][p.x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'P')
			{
				ft_bzero(&g->spider, sizeof(t_spidy));
				g->spider.x = p.x + 0.5;
				g->spider.y = p.y + 0.5;
				set_direction(g, c);
				return ;
			}
		}
	}
	ft_error_exit("Error: No player start position found in map\n");
}

static void	create_sprites(t_game *g)
{
	t_pos		p;
	t_sprite	*sprite;

	p.y = -1;
	while (g->map[++p.y])
	{
		p.x = -1;
		while (g->map[p.y][++p.x])
		{
			if (map[p.y][p.x] == 'L' || map[p.y][p.x] == 'B')
			{
				sprite = ft_safe_calloc(1, sizeof(t_sprite));
				sprite->x = p.x;
				sprite->y = p.y;
				if (map[p.y][p.x] == 'L')
				{
					sprite->over_scale = 1.5;
					ft_append_array((void ***)&g->lizards, sprite);
				}
				else
					ft_append_array((void ***)&g->bombs, sprite);
			}
		}
	}
}

static void	load_textures(t_game *g)
{
	g->spider.hand = malloc(sizeof(t_tex) * sizeof(t_state));
	if (!g->spider.hand)
		ft_error_exit("Error: Memory allocation failed for spider textures\n");
	load_texture(g, &g->spider.hand[ACTIVE], "./textures/spiderhand_01.xpm");
	load_texture(g, &g->spider.hand[MOVING], "./textures/spiderhand_02.xpm");
	load_texture(g, &g->spider.hand[ATTACKING],
		"./textures/spiderhand_attack.xpm");
	g->bomb_tex = malloc(sizeof(t_tex) * sizeof(t_state));
	if (!g->bomb_tex)
		ft_error_exit("Error: Memory allocation failed for bomb textures\n");
	load_texture(g, &g->bomb_tex[ACTIVE], "./textures/bomb.xpm");
	load_texture(g, &g->bomb_tex[ATTACKED], "./textures/bomb_attacked.xpm");
	g->lizard_tex = malloc(sizeof(t_tex) * sizeof(t_state));
	if (!g->lizard_tex)
		ft_error_exit("Error: Memory allocation failed for lizard textures\n");
	load_texture(g, &g->lizard_tex[ACTIVE], "./textures/lizard.xpm");
	load_texture(g, &g->lizard_tex[ATTACKED], "./textures/lizard_attacked.xpm");
	load_texture(g, &g->lizard_tex[ATTACKING],
		"./textures/lizard_attack.xpm");
	load_texture(g, &g->lizard_tex[MOVING],
		"./textures/lizard_step.xpm");
	/* Carga texturas: ajusta paths según tus archivos */
	load_texture(g, &g->floor, "./textures/floor.xpm");
	load_texture(g, &g->ceiling, "./textures/ceiling.xpm");
	load_texture(g, &g->wall_north, "./textures/wall.xpm");
	load_texture(g, &g->wall_south, "./textures/wall.xpm");
	load_texture(g, &g->wall_east, "./textures/wall.xpm");
	load_texture(g, &g->wall_west, "./textures/wall.xpm");
}

int	main(int argc, char **argv)
{
	int		fd;
	char	**content;
	t_game	g;

	ft_bzero(&g, sizeof(t_game));
	parse_arguments(argc, argv);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        error_handler(2);
    content = read_file(fd);
	if (!content)
		error_handler(3);



    parse_file(&g, content);
	g.map = ft_split(map_str, '\n');
	create_spiderman(&g);
	create_sprites(&g);
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
	load_textures(&g);
	ft_bzero(&g.keys, sizeof(t_keys));
	mlx_hook(g.win, 2, 1L << 0, key_press, &g);   // tecla presionada
	mlx_hook(g.win, 3, 1L << 1, key_release, &g); // tecla liberada
	g.show_intro = 1;
	mlx_loop_hook(g.mlx, render, &g); // loop continuo
	mlx_loop(g.mlx);
	close_program(&g);
	return (0);
}
