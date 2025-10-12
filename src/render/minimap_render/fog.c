/* ************************************************************************** */
/*                                                                            */
/*   minimap_fog.c                                      :+:      :+:    :+:   */
/*                                                                            */
/*   Gestión de la niebla (fog of war)                                        */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

extern char **revealed;
extern int last_w, last_h;

void free_revealed(void)
{
	int i = -1;
	if (!revealed)
		return;
	while (++i < last_h)
		free(revealed[i]);
	free(revealed);
	revealed = NULL;
}

void init_revealed_if_needed(int w, int h)
{
	int i;

	if (revealed && w == last_w && h == last_h)
		return;
	free_revealed();
	revealed = malloc(sizeof(char *) * h);
	if (!revealed)
		return;
	i = -1;
	while (++i < h)
	{
		revealed[i] = malloc(w);
		if (revealed[i])
			ft_memset(revealed[i], 0, w);
	}
	last_w = w;
	last_h = h;
}

void reveal_radius(int cx, int cy, int r, int w, int h)
{
	int y, x;

	y = fmax(0, cy - r) - 1;
	while (++y <= fmin(h - 1, cy + r))
	{
		x = fmax(0, cx - r) - 1;
		while (++x <= fmin(w - 1, cx + r))
			if (sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) <= r + 0.4)
				revealed[y][x] = 1;
	}
}

int is_revealed(int x, int y, int w, int h)
{
	if (!revealed || x < 0 || y < 0 || x >= w || y >= h)
		return (0);
	return (revealed[y][x]);
}
