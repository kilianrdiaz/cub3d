/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_system.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 20:25:37 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/13 20:27:33 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	reset_player_after_death(t_game *g)
{
	if (g->lives.lives_left > 0)
	{
		g->lives.lives_left--;
		ft_printf("💀 Vida perdida. Vidas restantes: %d\n", g->lives.lives_left);
		if (g->lives.lives_left > 0)
		{
			g->lives.player_hp = g->lives.player_max_hp;
			g->lives.player_last_hit_time = g->timer + 30;
		}
		else
			ft_printf("GAME OVER 💀\n");
		}
	}
	
int	lizard_on_player(t_game *g)
{
	int	i;

	if (!g->lizards)
		return (0);
	i = 0;
	while (g->lizards[i])
	{
		if (g->lizards[i]->dist < 0.8)
			return (1);
		i++;
	}
	return (0);
}

void	player_take_damage(t_game *g, int amount)
{
	if (g->lives.player_max_hp <= 0)
		g->lives.player_max_hp = 100;
	if (!lizard_on_player(g))
		return ;
	g->lives.player_last_hit_time = g->timer;
	g->lives.player_hp -= amount;
	if (g->lives.player_hp < 0)
		g->lives.player_hp = 0;
	if (g->lives.player_hp <= 0)
		reset_player_after_death(g);
}

