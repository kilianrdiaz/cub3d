/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_level.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:33:30 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/06 15:34:22 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static double	lerp(double a, double b, double t)
{
	return (a + (b - a) * t);
}

static t_ray	get_ray_animation(const char *msg, double x, double char_w)
{
	t_ray	ray;
	double	text_w;

	ray.row_distance = x;
	text_w = (ft_strlen(msg) * char_w);
	ray.coords.x = (WIDTH / 2) - (text_w / 2);
	ray.coords.y = HEIGHT / 2;
	ray.d_start.x = -text_w;
	ray.d_end.x = WIDTH;
	return (ray);
}

static t_coords	get_animation_coords(t_game *g, const char *msg,
		double secs_left)
{
	t_ray	ray;
	double	text_speed;

	ray.row_distance = TEXT_DURATION - secs_left;
	if (ray.row_distance < 0.0 || ray.row_distance > TEXT_DURATION)
		return ((t_coords){-1, -1});
	ray = get_ray_animation(msg, ray.row_distance, g->font.char_w
			* g->font.scale);
	text_speed = (TEXT_DURATION - TEXT_HOLD_TIME) / 2.0;
	if (ray.row_distance < text_speed)
	{
		ray.row_distance = ray.row_distance / text_speed;
		ray.coords.x = lerp(ray.d_start.x, ray.coords.x, ray.row_distance);
	}
	else if (ray.row_distance < text_speed + TEXT_HOLD_TIME)
		ray.coords.x = ray.coords.x;
	else
	{
		ray.row_distance = (ray.row_distance - (text_speed + TEXT_HOLD_TIME))
			/ text_speed;
		ray.coords.x = lerp(ray.coords.x, ray.d_end.x, ray.row_distance);
	}
	return (ray.coords);
}

static void	put_message(t_game *g, char *msg, int render_state)
{
	t_timeleft	t;
	t_coords	coords;

	coords = get_animation_coords(g, msg, TEXT_DURATION - g->timer);
	t = set_message(g, msg, coords);
	timeout_render(g, t, render_state);
	g->timer++;
}

void	load_level(t_game *g)
{
	char	*level;
	char	*msg;

	if (!g->timer && g->render_state == LOAD_LEVEL)
	{
		++g->levels;
		++g->level;
		get_info_file(g);
	}
	if (g->render_state == WIN)
		put_message(g, "WELL DONE! YOU ESCAPED!", HIGH_SCORE);
	if (g->render_state == GAME_OVER)
		put_message(g, "GAME OVER!", HIGH_SCORE);
	if (g->level == 1 && g->render_state == LOAD_LEVEL)
		put_message(g, "DEFUSE BOMBS!", NEW_LEVEL);
	if (g->render_state == NEW_LEVEL)
	{
		level = ft_itoa(g->level);
		msg = ft_strjoin("LEVEL ", level);
		free(level);
		put_message(g, msg, PLAYING);
		free(msg);
	}
}
