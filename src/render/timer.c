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

static t_timeleft	get_time_left(int timer)
{
	t_timeleft	t;
	int			elapsed_seconds;
	int			remaining;

	elapsed_seconds = timer / 30;
	remaining = TIMER - elapsed_seconds;
	if (remaining < 0)
		remaining = 0;
	t.minutes = remaining / 60;
	t.seconds = remaining % 60;
	return (t);
}

void	put_timer(t_game *g, t_pos pos)
{
	t_timeleft	t;
	char		*str;
	char		**buffer;

	t = get_time_left(g->timer);
	buffer = NULL;
	str = ft_itoa(t.minutes);
	ft_append_array((void ***)&buffer, str);
	str = ft_strdup(":");
	ft_append_array((void ***)&buffer, str);
	str = ft_itoa(t.seconds);
	ft_append_array((void ***)&buffer, str);
	render_text(g, buffer[0], pos);
	pos.x += (int)(g->font.char_w);
	render_text(g, buffer[1], pos);
	pos.x += (int)(g->font.char_w);
	if (t.seconds < 10)
	{
		render_text(g, "0", pos);
		pos.x += (int)(g->font.char_w * g->font.scale);
	}
	render_text(g, buffer[2], pos);
	ft_free_array((void ***)&buffer);
}

void	update_timer(t_game *g)
{
    t_timeleft    t;

    t = get_time_left(g->timer);
    if (t.minutes == 0 && t.seconds == 0)
    {
        g->render_state = HIGH_SCORE;
        return ;
    }
    g->timer++;
}
