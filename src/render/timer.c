/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:10:55 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/03 14:11:03 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#ifndef FRAMES_PER_SECOND
# define FRAMES_PER_SECOND 10
#endif

static t_timeleft	get_time_left(unsigned int timer, unsigned int time_limit)
{
	t_timeleft	t;
	int			elapsed_seconds;
	int			remaining;

	elapsed_seconds = timer / FRAMES_PER_SECOND;
	remaining = time_limit - elapsed_seconds;
	if (remaining < 0)
		remaining = 0;
	t.minutes = remaining / 60;
	t.seconds = remaining % 60;
	return (t);
}

void	put_timer(t_game *g, t_coords coords)
{
	t_timeleft	t;
	char		*str;
	char		**buffer;

	t = get_time_left(g->timer, g->timeout);
	buffer = NULL;
	str = ft_itoa(t.minutes);
	ft_append_array((void ***)&buffer, str);
	str = ft_strdup(":");
	ft_append_array((void ***)&buffer, str);
	str = ft_itoa(t.seconds);
	ft_append_array((void ***)&buffer, str);
	render_text(g, buffer[1], coords);
	if (buffer[0])
		coords.x -= (g->font.char_w * g->font.scale) * ft_strlen(buffer[0]);
	render_text(g, buffer[0], coords);
	coords.x = GAME_W + ((WIDTH - GAME_W) / 2) - 60;
	coords.x += (g->font.char_w * g->font.scale);
	if (t.seconds < 10)
	{
		render_char(g, '0', coords);
		coords.x += (g->font.char_w * g->font.scale);
	}
	render_text(g, buffer[2], coords);
	ft_free_array((void ***)&buffer);
}

void	update_timer(t_game *g)
{
	t_timeleft	t;

	if (g->render_state != PLAYING)
		return ;
	t = get_time_left(g->timer, g->timeout);
	if (t.minutes == 0 && t.seconds == 0)
	{
		g->render_state = GAME_OVER;
		g->timer = 0;
		return ;
	}
	g->timer++;
}

t_timeleft	set_message(t_game *g, char *msg, t_coords coords)
{
	t_timeleft	t;
	double		timer;

	timer = g->timer;
	g->timer = g->timeout;
	game(g);
	g->timer = timer;
	t = get_time_left(g->timer, TEXT_DURATION);
	if (coords.x < 0 && coords.y < 0)
		return ((t_timeleft){0, 0});
	else
		render_text(g, msg, coords);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	return (t);
}
