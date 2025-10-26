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
	int		s;
	int		sx;
	int		sy;
	int		mx;
	int		my;

	mx = (int)sp->x;
	my = (int)sp->y;
	if (!m->revealed || mx < 0 || my < 0
		|| mx >= m->width || my >= m->height
		|| !m->revealed[my][mx])
		return ;
	sx = inf->ox + sp->x * inf->t;
	sy = inf->oy + sp->y * inf->t;
	s = fmax(2, inf->t / 2);
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



