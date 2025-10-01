/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bomb.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 14:31:31 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/01 14:31:34 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	update_bombs(t_game *g)
{
	int	i;

	i = -1;
	while (++i < MAP_W * MAP_H)
	{
		if (g->bombs[i] && g->bombs[i]->state == ATTACKED)
		{
			// Aquí puedes añadir lógica adicional, como un temporizador
			// para cambiar el estado a DEFUSED después de un tiempo.
			g->bombs[i]->state = DEFUSED;
			printf("Bomb at (%.1f, %.1f) has been defused.\n", g->bombs[i]->x,
				g->bombs[i]->y);
			free(g->bombs[i]->tex);
            return ;
		}
	}
}

static void	create_bomb(t_game *g, t_pos p)
{
	t_sprite	*bomb;
	int			idx;

	idx = p.x + p.y * MAP_W;
	bomb = malloc(sizeof(t_sprite));
	if (!bomb)
		ft_error_exit("Error: Memory allocation failed for bomb\n");
	ft_bzero(bomb, sizeof(t_sprite));
	bomb->x = p.x;
	bomb->y = p.y;
	bomb->tex = malloc(sizeof(t_tex) * sizeof(t_state));
	if (!bomb->tex)
		ft_error_exit("Error: Memory allocation failed for bomb textures\n");
	load_texture(g, &bomb->tex[ACTIVE], "./textures/bomb.xpm");
	load_texture(g, &bomb->tex[ATTACKED], "./textures/bomb_attacked.xpm");
	g->bombs[idx] = bomb;
}

void	init_bombs(t_game *g)
{
	t_pos	p;
	int		total;

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
				create_bomb(g, p);
		}
	}
}
