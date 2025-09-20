/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:22:09 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/19 16:22:11 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	sort_sprites(t_sprite_order *order, int count)
{
	t_sprite_order	tmp;

	int i, j;
	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (order[i].dist < order[j].dist)
			{
				tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
			}
		}
	}
}

void draw_sprite(t_game *g, t_sprite *sp, double transformY, double transformX)
{
    int spriteHeight, spriteWidth;
    int drawStartY, drawEndY, drawStartX, drawEndX;
    int texX, texY, d;
    unsigned int color;

    // 1️⃣ Altura y ancho del sprite según la distancia (como las paredes)
    spriteHeight = abs((int)(BOMB_H / transformY));
    spriteWidth  = abs((int)(spriteHeight * ((double)sp->tex.width / sp->tex.height)));

    // 2️⃣ Offset vertical para apoyarlo en el suelo
    int vMoveScreen = (int)(HEIGHT / transformY * 0.5);

    // 3️⃣ Límites verticales
    drawStartY = -spriteHeight / 2 + HEIGHT / 2 + vMoveScreen;
    drawEndY   =  spriteHeight / 2 + HEIGHT / 2 + vMoveScreen;
    if (drawStartY < 0) drawStartY = 0;
    if (drawEndY >= HEIGHT) drawEndY = HEIGHT - 1;

    // 4️⃣ Límites horizontales
    int spriteScreenX = (int)((WIDTH / 2) * (1 + transformX / transformY));
    drawStartX = -spriteWidth / 2 + spriteScreenX;
    drawEndX   = drawStartX + spriteWidth;
    if (drawStartX < 0) drawStartX = 0;
    if (drawEndX >= WIDTH) drawEndX = WIDTH - 1;

    // 5️⃣ Dibujar sprite con z-buffer
    for (int stripe = drawStartX; stripe < drawEndX; stripe++)
    {
        if (transformY > 0 && stripe >= 0 && stripe < WIDTH && transformY < g->zbuffer[stripe])
        {
            texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * sp->tex.width / spriteWidth) / 256;

            for (int y = drawStartY; y < drawEndY; y++)
            {
                d = (y - drawStartY) * 256;
                texY = ((d * sp->tex.height) / spriteHeight) / 256;

                if (texX < 0 || texX >= sp->tex.width || texY < 0 || texY >= sp->tex.height)
                    continue;

                color = *(unsigned int *)(sp->tex.addr + texY * sp->tex.line_len + texX * (sp->tex.bpp / 8));
                if ((color & 0x00FFFFFF) != 0) // Transparencia
                    put_pixel(g, stripe, y, color);
            }
        }
    }
}

void render_sprites(t_game *g)
{
    t_sprite_order order[MAP_W * MAP_H];
    int count = 0;
    int idx;
    double dx, dy;
    t_sprite *sp;

    // 1️⃣ Recolectar todas las bombas
    for (int y = 0; y < MAP_H; y++)
    {
        for (int x = 0; x < MAP_W; x++)
        {
            idx = x + y * MAP_W;
            if (g->bombs[idx] != NULL)
            {
                order[count].index = idx;
                dx = g->spider.posX - g->bombs[idx]->x;
                dy = g->spider.posY - g->bombs[idx]->y;
                order[count].dist = dx * dx + dy * dy; // distancia al jugador
                count++;
            }
        }
    }

    // 2️⃣ Ordenar de más lejos a más cerca
    sort_sprites(order, count);

    // 3️⃣ Dibujar cada sprite
    for (int i = 0; i < count; i++)
    {
        sp = g->bombs[order[i].index];
        double spriteX = sp->x + 0.5 - g->spider.posX;
        double spriteY = sp->y + 0.5 - g->spider.posY;

        // Transformar a espacio de cámara
        double invDet = 1.0 / (g->spider.planeX * g->spider.dirY - g->spider.dirX * g->spider.planeY);
        double transformX = invDet * (g->spider.dirY * spriteX - g->spider.dirX * spriteY);
        double transformY = invDet * (-g->spider.planeY * spriteX + g->spider.planeX * spriteY);

        // Solo renderizar si está delante del jugador
        if (transformY > 0.0)
            draw_sprite(g, sp, transformY, transformX);
    }
}

