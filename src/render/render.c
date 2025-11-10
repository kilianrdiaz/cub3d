/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/29 21:55:38 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	game(t_game *g)
{
	update_player_position(g);
	clean_screen(g);
	render_floor_and_ceiling(g);
	render_wall(g);
	render_sprites(g);
	move_lizards(g);
	draw_hand(g, GAME_WIDTH / 2);
	draw_minimap(g);
	render_stats(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	update_timer(g);
	update_bombs(g);
	if (g->spider.state == ATTACKING)
		spider_attack(g);
}

int	render(t_game *g)
{
	if (g->render_state == INTRO)
		show_intro(g);
	else if (g->render_state == LOAD_LEVEL || g->render_state == NEW_LEVEL
		|| g->render_state == GAME_OVER)
		load_level(g);
	else if (g->render_state == PLAYING)
		game(g);
	else if (g->render_state == HIGH_SCORE
		|| g->render_state == WAITING_FOR_NAME
		|| g->render_state == SCORE_SAVED)
		show_high_scores(g);
	else if (g->render_state == END)
		close_program(g);
	return (0);
}
