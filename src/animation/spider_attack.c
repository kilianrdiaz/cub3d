/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider_attack.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:49:27 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/03 18:49:29 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

void	update_bombs(t_game *g)
{
	int	i;

	i = -1;
	if (g->bomb_count <= 0)
		return ;
	while (g->bombs[++i])
	{
		if (g->bombs[i]->state == ATTACKED)
		{
			g->bombs[i]->state = DEFUSED;
			printf("Bomb at (%.1f, %.1f) has been defused.\n", g->bombs[i]->x,
				g->bombs[i]->y);
			g->bomb_count--;
		}
	}
}

static int	check_if_is_lizard(t_game *g, int x, int y)
{
	int		i;
	t_pos	t;

	i = -1;
	while (g->lizards && g->lizards[++i])
	{
		t.x = (int)g->lizards[i]->x;
		t.y = (int)g->lizards[i]->y;
		if (t.x == x && t.y == y)
		{
			g->lizards[i]->state = ATTACKED;
			g->lizards[i]->delay = g->timer + 20;
			g->keys.space = 0;
			ft_printf("Lizard attacked at (%d,%d)\n", t.x, t.y);
			return (1);
		}
	}
	return (0);
}

static int	check_if_is_bomb(t_game *g, int x, int y)
{
	int		i;
	t_pos	t;

	i = -1;
	while (g->bombs && g->bombs[++i])
	{
		t.x = (int)g->bombs[i]->x;
		t.y = (int)g->bombs[i]->y;
		if (t.x == x && t.y == y && g->bombs[i]->state != DEFUSED)
		{
			g->bombs[i]->state = ATTACKED;
			g->keys.space = 0;
			ft_printf("Bomb attacked at (%d,%d)\n", t.x, t.y);
			return (1);
		}
	}
	return (0);
}

void	spider_attack(t_game *g)
{
	int		i;
	t_pos	t;

	i = 0;
	while (++i <= 4)
	{
		t.x = (int)g->spider.x + (int)(g->spider.dir_x * i);
		t.y = (int)g->spider.y + (int)(g->spider.dir_y * i);
		if (t.x < 0 || t.y < 0)
			break ;
		if (check_if_is_lizard(g, t.x, t.y) || check_if_is_bomb(g, t.x, t.y))
			break ;
		if (g->map[t.y][t.x] == '1')
			break ;
	}
}
