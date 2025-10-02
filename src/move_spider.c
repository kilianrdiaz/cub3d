/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_spider.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:14:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/20 12:27:47 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_walkable(int x, int y)
{
	if (x < 0 || y < 0)
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

static void	move_player(t_game *g, double dir)
{
	double	new_x;
	double	new_y;
	double	distance;

	new_x = g->spider.x + g->spider.dir_x * dir * MOVE_SPEED;
	new_y = g->spider.y + g->spider.dir_y * dir * MOVE_SPEED;
	distance = sqrt(pow(new_x - g->spider.x, 2) + pow(new_y - g->spider.y, 2));
	// distancia recorrida en este frame
	// Comprobar colisiones
	if (is_walkable((int)new_x, (int)g->spider.y))
		g->spider.x = new_x;
	if (is_walkable((int)g->spider.x, (int)new_y))
		g->spider.y = new_y;
	// Acumular distancia recorrida
	g->spider.move_accum += distance;
	// Si recorrió más de STEP_DISTANCE, cambiar sprite
	if (g->spider.move_accum >= 0.40)
	{
		if (g->spider.state == ACTIVE)
			g->spider.state = MOVING;
		else if (g->spider.state == MOVING)
			g->spider.state = ACTIVE;
		g->spider.move_accum = 0.0; // reset
	}
}

static void	rotate_spidy(t_spidy *spidy, double angle)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = spidy->dir_x;
	oldplane_x = spidy->plane_x;
	// Rotación con ángulo pequeño (radianes)
	spidy->dir_x = spidy->dir_x * cos(angle) - spidy->dir_y * sin(angle);
	spidy->dir_y = olddir_x * sin(angle) + spidy->dir_y * cos(angle);
	spidy->plane_x = spidy->plane_x * cos(angle) - spidy->plane_y * sin(angle);
	spidy->plane_y = oldplane_x * sin(angle) + spidy->plane_y * cos(angle);
}

void	update_player_position(t_game *g)
{
	if (g->keys.a) // rotar izq
		rotate_spidy(&g->spider, -ROT_SPEED);
	if (g->keys.d) // rotar der
		rotate_spidy(&g->spider, ROT_SPEED);
	if (g->keys.w) // adelante
		move_player(g, 1.0);
	if (g->keys.s) // atrás
		move_player(g, -1.0);
	if (g->keys.space) // atacar
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
}

void	spider_attack(t_game *g)
{
	int		i;
	t_pos	t;
	int		idx;

	i = 0;
	while (++i <= 4)
	{
		t.x = (int)g->spider.x + (int)(g->spider.dir_x * i);
		t.y = (int)g->spider.y + (int)(g->spider.dir_y * i);
		if (t.x < 0 || t.x >= MAP_W || t.y < 0 || t.y >= MAP_H)
			break ;
		idx = t.x + t.y * MAP_W;
		if (g->bombs[idx] && g->bombs[idx]->state != DEFUSED)
		{
			g->bombs[idx]->state = ATTACKED;
			g->bomb_count--;
			g->keys.space = 0; // Evita múltiples ataques
			printf("Bomb attacked at (%d,%d)\n", t.x, t.y);
			break ;
		}
		if (map[t.y][t.x] == '1')
			break ;
	}
}
