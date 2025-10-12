/* ************************************************************************** */
/*                                                                            */
/*   minimap_sprites.c                                 :+:      :+:    :+:   */
/*                                                                            */
/*   Dibujo de bombas y enemigos en el minimapa                              */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

extern int last_w, last_h;
extern int is_revealed(int x, int y, int w, int h);

void draw_sprites_minimap(t_game *g, t_sprite **arr, int t, int ox, int oy, int color)
{
	int i, sx, sy, s;

	i = -1;
	while (arr && arr[++i])
	{
		int mx = (int)arr[i]->x;
		int my = (int)arr[i]->y;
		if (!is_revealed(mx, my, last_w, last_h))
			continue;
		sx = ox + arr[i]->x * t;
		sy = oy + arr[i]->y * t;
		s = fmax(2, t / 2);
		put_rect(g, sx - s / 2, sy - s / 2, s, s, color);
	}
}
