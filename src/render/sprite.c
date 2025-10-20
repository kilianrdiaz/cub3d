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
	t_pos			pos;

	pos.x = -1;
	while (++pos.x < count)
	{
		pos.y = pos.x;
		while (++pos.y < count)
		{
			if (order[pos.x].dist < order[pos.y].dist)
			{
				tmp = order[pos.x];
				order[pos.x] = order[pos.y];
				order[pos.y] = tmp;
			}
		}
	}
}

static void	draw_sprite(t_game *g, t_sprite *sp, t_ray ray, t_tex *tex)
{
	int	d;
	int	y;

	if (sp->state == DEFUSED)
		return ;
	ray.src.x = (int)(256 * (ray.line_height - (-sp->width / 2 + sp->screen_x))
			* tex[sp->state].width / sp->width) / 256;
	y = ray.d_start.y - 1;
	while (++y < ray.d_end.y)
	{
		d = (y - ray.d_start.y) * 256;
		ray.src.y = ((d * tex[sp->state].height) / sp->height) / 256;
		if (ray.src.x < 0 || ray.src.x >= tex[sp->state].width || ray.src.y < 0
			|| ray.src.y >= tex[sp->state].height)
			continue ;
		ray.color = *(unsigned int *)(tex[sp->state].addr + ray.src.y
				* tex[sp->state].line_len + ray.src.x * (tex[sp->state].bpp
					/ 8));
		if ((ray.color & 0x00FFFFFF) != 0) // Transparencia
			put_pixel(g, ray.line_height, y, ray.color);
	}
}

static void	ray_sprite(t_sprite *sp, t_ray *ray, t_tex *tex)
{
	// 1️⃣ Altura y ancho del sprite según la distancia (como las paredes)
	sp->height = (tex[sp->state].height * sp->scale / sp->trans_y);
	sp->width = (tex[sp->state].width * sp->scale / sp->trans_y);
	// 2️⃣ Offset vertical para apoyarlo en el suelo
	ray->view = (int)(HEIGHT / sp->trans_y * 0.5);
	// 3️⃣ Límites verticales
	ray->d_start.y = -sp->height / 2 + HEIGHT / 2 + ray->view;
	ray->d_end.y = sp->height / 2 + HEIGHT / 2 + ray->view;
	if (ray->d_start.y < 0)
		ray->d_start.y = 0;
	if (ray->d_end.y >= HEIGHT)
		ray->d_end.y = HEIGHT - 1;
	// 4️⃣ Límites horizontales
	sp->screen_x = (int)((GAME_WIDTH / 2) * (1 + sp->trans_x / sp->trans_y));
	ray->d_start.x = -sp->width / 2 + sp->screen_x;
	ray->d_end.x = ray->d_start.x + sp->width;
	if (ray->d_start.x < 0)
		ray->d_start.x = 0;
	if (ray->d_end.x >= GAME_WIDTH)
		ray->d_end.x = GAME_WIDTH - 1;
}

static void	position_sprite(t_game *g, t_sprite sp)
{
	t_ray	ray;
	t_tex	*tex;

	if (sp.state == DEFUSED)
		return ;
	tex = g->bomb_tex;
	if (sp.type != BOMB)
		tex = g->lizard_tex;
	sp.x = sp.x + 0.5 - g->spider.x;
	sp.y = sp.y + 0.5 - g->spider.y;
	sp.inv_det = 1.0 / (g->spider.plane_x * g->spider.dir_y - g->spider.dir_x
			* g->spider.plane_y);
	sp.trans_x = sp.inv_det * (g->spider.dir_y * sp.x - g->spider.dir_x * sp.y);
	sp.trans_y = sp.inv_det * (-g->spider.plane_y * sp.x + g->spider.plane_x
			* sp.y);
	if (sp.trans_y > 0.0)
	{
		ray_sprite(&sp, &ray, tex);
		ray.line_height = ray.d_start.x - 1;
		while (++ray.line_height < ray.d_end.x)
			if (sp.trans_y > 0 && ray.line_height >= 0
				&& ray.line_height < GAME_WIDTH
				&& sp.trans_y < g->zbuffer[ray.line_height])
				draw_sprite(g, &sp, ray, tex);
	}
}

void	render_sprites(t_game *g)
{
	t_sprite_order	*order;
	t_pos			count;
	t_sprite		**sprites;

	sprites = get_sprites(g);
	if (!sprites)
		return ;
	count.x = ft_memlen((void **)sprites);
	order = malloc(sizeof(t_sprite_order) * count.x);
	if (!order)
		ft_error_exit("Error: Memory allocation failed for sprite order\n");
	count.y = -1;
	while (sprites[++count.y])
	{
		order[count.y].index = count.y;
		order[count.y].dist = pow(g->spider.x - sprites[count.y]->x, 2)
			+ pow(g->spider.y - sprites[count.y]->y, 2);
		recalc_sprite_scale(g, sprites[count.y], sqrt(order[count.y].dist));
	}
	sort_sprites(order, count.x);
	count.y = -1;
	while (++count.y < count.x)
		position_sprite(g, *sprites[order[count.y].index]);
	free(order);
	free(sprites);
}
