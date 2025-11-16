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

#define DAMAGE_AMOUNT 5

static void	reset_player_after_death(t_game *g)
{
	g->live.lives_left--;
	if (g->live.lives_left > 0)
		g->live.player_hp = 100;
	else
	{
		g->render_state = GAME_OVER;
		g->timer = 0;
	}
}

void	player_take_damage(t_game *g, t_sprite *lizard)
{
	lizard->state = ATTACKING;
	g->live.player_hp -= DAMAGE_AMOUNT;
	if (g->live.player_hp < 0)
		g->live.player_hp = 0;
	if (g->live.player_hp <= 0)
		reset_player_after_death(g);
}
