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

#define TEXT_DURATION 7.0
#define TEXT_HOLD_TIME 1

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

static void	render_text_animation(t_game *g, const char *msg, double secs_left)
{
	t_ray	ray;
	double	text_speed;

	ray.row_distance = TEXT_DURATION - secs_left;
	if (ray.row_distance < 0.0 || ray.row_distance > TEXT_DURATION)
		return ;
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
	render_text(g, (char *)msg, ray.coords);
}

static t_timeleft	set_message(t_game *g, char *msg)
{
	t_timeleft	t;
	double		timer;

	clean_screen(g);
	render_floor_and_ceiling(g);
	render_wall(g);
	render_sprites(g);
	draw_hand(g, GAME_WIDTH / 2);
	draw_minimap(g);
	timer = g->timer;
	g->timer = 0;
	render_stats(g);
	g->timer = timer;
	t = get_time_left(g->timer, TEXT_DURATION);
	render_text_animation(g, msg, t.minutes * 60 + t.seconds);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	return (t);
}

static void new_level_setup(t_game *g)
{
	t_timeleft	t;
	char		*nb;
	char		*msg;

	if (g->level == 1 && g->render_state == LOAD_LEVEL)
	{
		t = set_message(g, "DEFUSE THE BOMBS!");
		if (t.minutes == 0 && t.seconds == 0)
		{
			g->render_state = NEW_LEVEL;
			g->timer = 0;
		}
		return ;
	}
	nb = ft_itoa(g->level);
	msg = ft_strjoin("LEVEL ", nb);
	t = set_message(g, msg);
	free(nb);
	free(msg);
	if (t.minutes == 0 && t.seconds == 0)
	{
		g->render_state = PLAYING;
		g->timer = 0;
	}
}

int	load_level(t_game *g)
{
	if (!g->timer && g->render_state == LOAD_LEVEL)
	{
		++g->levels;
		++g->level;
		if (ft_memlen(g->levels))
			get_info_file(g);
		else
			return (g->render_state = HIGH_SCORE, 0);
	}
	if (g->render_state == HIGH_SCORE)
		return (0);
	new_level_setup(g);
	g->timer++;
	return (0);
}
