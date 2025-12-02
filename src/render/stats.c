/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:59:06 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/15 20:20:59 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define MIDELINE_WIDTH 8
#define MIDELINE_COLOR 0xFF555555

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
			src_pos.x = (int)(p.x / sp.scale);
			src_pos.y = (int)(p.y / sp.scale);
			if (src_pos.x < 0 || src_pos.x >= tex.width || src_pos.y < 0
				|| src_pos.y >= tex.height)
				continue ;
			src = tex.addr + (src_pos.y * tex.line_len + src_pos.x * (tex.bpp
						/ 8));
			tex.color = *(unsigned int *)src;
			if ((tex.color & 0x00FFFFFF) != 0)
				put_pixel(g, (int)sp.pos.x + p.x, (int)sp.pos.y + p.y,
					tex.color);
		}
	}
}

static void	put_score_text(t_game *g, t_coords pos)
{
	char	*str;
	int		i;

	pos.x = GAME_W + ((WIDTH - GAME_W) / 2) - 60;
	pos.x += (g->font.char_w * g->font.scale) * 2;
	if (!g->score)
		return (render_char(g, '0', pos));
	str = ft_itoa(g->score);
	if (!str)
		return ;
	i = ft_strlen(str);
	while (--i >= 0)
	{
		render_char(g, str[i], pos);
		pos.x -= g->font.char_w;
	}
	free(str);
}

void	draw_panel_separator(t_game *g)
{
	t_pos	p;

	p.x = GAME_W - 1;
	while (++p.x < GAME_W + 8)
	{
		p.y = -1;
		while (++p.y < HEIGHT)
			put_pixel(g, p.x, p.y, 0xFF555555);
	}
}

static void	render_live(t_game *g)
{
	t_sprite	sp;

	sp.pos.x = GAME_W + 80;
	sp.pos.y = 100;
	sp.scale = 0.25;
	draw_text(g, g->live.spidermask_tex[g->spider.spider_sense], sp);
	if (g->live.lives_left > 1)
	{
		sp.pos.x = GAME_W + 370;
		sp.pos.y = 55;
		sp.scale = 0.15;
		draw_text(g, g->live.spidermask_tex[0], sp);
	}
	if (g->live.lives_left > 2)
	{
		sp.pos.x = GAME_W + 290;
		sp.pos.y = 55;
		sp.scale = 0.15;
		draw_text(g, g->live.spidermask_tex[0], sp);
	}
}

void	render_stats(t_game *g)
{
	t_sprite	sp;
	char		*str;

	draw_health_bar(g);
	draw_panel_separator(g);
	sp.pos.x = GAME_W + ((WIDTH - GAME_W) / 2) - 120;
	sp.pos.y = HEIGHT - 250;
	sp.scale = 0.5;
	draw_text(g, g->bomb_tex[ACTIVE], sp);
	sp.pos.x = GAME_W + ((WIDTH - GAME_W) / 2) + 120;
	g->font.scale = 1.8;
	sp.pos.y -= g->font.char_h * g->font.scale / 3;
	str = ft_itoa(g->bomb_count);
	if (str)
		sp.pos.x -= (g->font.char_w * g->font.scale * ft_strlen(str)) / 2;
	render_text(g, str, sp.pos);
	free(str);
	sp.pos.x = GAME_W + ((WIDTH - GAME_W) / 2) - 60;
	sp.pos.y += g->bomb_tex[ACTIVE].height * sp.scale + 50;
	put_timer(g, sp.pos);
	sp.pos.y += g->bomb_tex[ACTIVE].height * sp.scale + 50;
	put_score_text(g, sp.pos);
	render_live(g);
	draw_minimap(g);
}
