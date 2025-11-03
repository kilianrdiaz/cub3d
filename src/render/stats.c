/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:59:06 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/02 20:59:09 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

typedef struct s_timeleft
{
	int				minutes;
	int				seconds;
}					t_timeleft;

static void	draw_text(t_game *g, t_tex tex, t_sprite sp)
{
	t_pos	p;
	t_pos	src_pos;
	char	*src;

	p.y = -1;
	while (++p.y < tex.height * sp.scale)
	{
		p.x = -1;
		while (++p.x < tex.width * sp.scale)
		{
			// Convertir coordenadas escaladas -> coordenadas reales de textura
			src_pos.x = (int)(p.x / sp.scale);
			src_pos.y = (int)(p.y / sp.scale);
			// Proteger límites
			if (src_pos.x < 0 || src_pos.x >= tex.width || src_pos.y < 0
				|| src_pos.y >= tex.height)
				continue ;
			src = tex.addr + (src_pos.y * tex.line_len + src_pos.x * (tex.bpp
						/ 8));
			tex.color = *(unsigned int *)src;
			if ((tex.color & 0x00FFFFFF) != 0) // transparencia
				put_pixel(g, (int)sp.pos.x + p.x, (int)sp.pos.y + p.y,
					tex.color);
		}
	}
}

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

static void	put_timer(t_game *g, t_pos pos)
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

static void	put_score_text(t_game *g, t_pos pos)
{
	int		advance;
	int		i;
	char	*str;
	char	ch;

	str = ft_itoa(g->score);
	if (!str)
		return ;
	pos.x -= (int)(g->font.char_w * ft_strlen(str) / 2);
	advance = (int)(g->font.char_w);
	i = -1;
	while (str[++i])
	{
		ch = str[i];
		render_text(g, &ch, (t_pos){pos.x + i * advance, pos.y});
	}
	free(str);
}

void	render_stats(t_game *g)
{
	t_sprite	sp;
	char		*str;

	sp.pos.x = GAME_WIDTH + 150;
	sp.pos.y = HEIGHT - 300;
	sp.scale = 0.5;
	draw_text(g, g->bomb_tex[ACTIVE], sp);
	sp.pos.x += g->bomb_tex[ACTIVE].width * sp.scale + 50;
	g->font.scale = 1.8;
	sp.pos.y -= g->font.char_h * g->font.scale / 3;
	str = ft_itoa(g->bomb_count);
	render_text(g, str, (t_pos){sp.pos.x, sp.pos.y});
	free(str);
	sp.pos.x = GAME_WIDTH + 150;
	sp.pos.y += g->bomb_tex[ACTIVE].height * sp.scale + 50;
	put_timer(g, (t_pos){sp.pos.x, sp.pos.y});
	sp.pos.y += g->font.char_h * g->font.scale + 10;
	sp.pos.x = GAME_WIDTH + 100 + g->bomb_tex[ACTIVE].width * sp.scale + 50;
	put_score_text(g, (t_pos){sp.pos.x, sp.pos.y});
}
