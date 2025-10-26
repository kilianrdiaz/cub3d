/* ************************************************************************** */
/*                                                                            */
/*   minimap_sprites.c                                 :+:      :+:    :+:   */
/*                                                                            */
/*   Dibujo de bombas y enemigos en el minimapa                              */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	draw_single_sprite(t_game *g, t_minimap *m,
		t_sprite *sp, t_sprite_info *inf)
{
	int	sx;
	int	sy;
	int	s;
	int	mx;
	int	my;

	if (!sp || sp->state == DEFUSED)
		return;

	mx = (int)sp->x;
	my = (int)sp->y;
	if (!m->revealed || mx < 0 || my < 0
		|| mx >= m->width || my >= m->height
		|| !m->revealed[my][mx])
		return ;

	sx = inf->ox + (int)(sp->x * inf->t + inf->t / 2);
	sy = inf->oy + (int)(sp->y * inf->t + inf->t / 2);
	s = (int)(inf->t * 0.5);
	if (s < 2)
		s = 2;

	put_rect(g, sx - s / 2, sy - s / 2, s, s, inf->color);
}



void	draw_sprites_minimap(t_game *g, t_minimap *m,
		t_sprite **arr, t_sprite_info *inf)
{
	int		i;

	i = -1;
	while (arr && arr[++i])
		draw_single_sprite(g, m, arr[i], inf);
}



