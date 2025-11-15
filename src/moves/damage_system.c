/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   damage_system.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 20:25:37 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 20:18:24 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	reset_player_after_death(t_game *g)
{
	if (g->lives.lives_left > 0)
	{
		g->lives.lives_left--;
		if (g->lives.lives_left >= 0)
			g->lives.player_hp = 100;
		else
			ft_printf("GAME OVER 💀\n");
	}
}

void	player_take_damage(t_game *g, t_sprite *lizard)
{
	lizard->state = ATTACKING;
	g->lives.player_hp -= 5;
	if (g->lives.player_hp < 0)
		g->lives.player_hp = 0;
	if (g->lives.player_hp <= 0)
		reset_player_after_death(g);
}
