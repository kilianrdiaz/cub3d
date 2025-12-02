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

static void	draw_sprite(t_game *g, t_sprite *sp, t_ray ray, t_tex tex)
{
	int				d;
	int				y;
	unsigned int	color;

	if (sp->state == DEFUSED)
		return ;
	ray.src.x = (int)(256 * (ray.line_height - (-sp->width / 2 + sp->screen_x))
			* tex.width / sp->width) / 256;
	y = ray.d_start.y - 1;
	while (++y < ray.d_end.y)
	{
		d = (y - ray.d_start.y) * 256;
		ray.src.y = ((d * tex.height) / sp->height) / 256;
		if (ray.src.x < 0 || ray.src.x >= tex.width || ray.src.y < 0
			|| ray.src.y >= tex.height)
			continue ;
		color = get_pixel_color(tex, ray.src.x, ray.src.y);
		if ((color & 0x00FFFFFF) != 0)
			put_pixel(g, ray.line_height, y, color);
	}
}

static void	ray_sprite(t_sprite *sp, t_ray *ray, t_tex tex)
{
	sp->height = (tex.height * sp->scale / sp->trans.y);
	sp->width = (tex.width * sp->scale / sp->trans.y);
	ray->view = (int)(HEIGHT / sp->trans.y * 0.5);
	ray->d_start.y = -sp->height / 2 + HEIGHT / 2 + ray->view;
	ray->d_end.y = sp->height / 2 + HEIGHT / 2 + ray->view;
	if (ray->d_start.y < 0)
		ray->d_start.y = 0;
	if (ray->d_end.y >= HEIGHT)
		ray->d_end.y = HEIGHT - 1;
	sp->screen_x = (int)((GAME_W / 2) * (1 + sp->trans.x / sp->trans.y));
	ray->d_start.x = -sp->width / 2 + sp->screen_x;
	ray->d_end.x = ray->d_start.x + sp->width;
	if (ray->d_start.x < 0)
		ray->d_start.x = 0;
	if (ray->d_end.x >= GAME_W)
		ray->d_end.x = GAME_W - 1;
	ray->line_height = ray->d_start.x - 1;
}

static void	position_sprite(t_game *g, t_sprite sp)
{
	t_ray	ray;
	t_tex	tex;

	if (sp.state == DEFUSED || sp.state == NO_RENDER)
		return ;
	tex = g->bomb_tex[sp.state];
	if (sp.type != BOMB)
		tex = g->lizard_tex[sp.state];
	sp.pos.x = sp.pos.x + 0.5 - g->spider.pos.x;
	sp.pos.y = sp.pos.y + 0.5 - g->spider.pos.y;
	sp.inv_det = 1.0 / (g->spider.plane.x * g->spider.dir.y - g->spider.dir.x
			* g->spider.plane.y);
	sp.trans.x = sp.inv_det * (g->spider.dir.y * sp.pos.x - g->spider.dir.x
			* sp.pos.y);
	sp.trans.y = sp.inv_det * (-g->spider.plane.y * sp.pos.x + g->spider.plane.x
			* sp.pos.y);
	if (sp.trans.y > 0.0 && tex.img && tex.addr)
	{
		ray_sprite(&sp, &ray, tex);
		while (++ray.line_height < ray.d_end.x)
			if (sp.trans.y > 0 && ray.line_height >= 0
				&& ray.line_height < GAME_W
				&& sp.trans.y < g->zbuffer[ray.line_height])
				draw_sprite(g, &sp, ray, tex);
	}
}

void	render_sprites(t_game *g)
{
	t_sprite_order	*order;
	t_pos			count;
	t_sprite		**sprites;

	sprites = get_sprites(*g);
	if (!sprites)
		return ;
	count.x = ft_memlen((void **)sprites);
	order = malloc(sizeof(t_sprite_order) * count.x);
	if (!order)
		ft_error_exit("Error: Memory allocation failed for sprite order\n");
	count.y = -1;
	while (sprites[++count.y])
	{
		recalc_sprite_scale(g, sprites[count.y]);
		order[count.y].index = count.y;
		order[count.y].dist = sprites[count.y]->dist;
	}
	sort_sprites(order, count.x);
	count.y = -1;
	while (++count.y < count.x)
		position_sprite(g, *sprites[order[count.y].index]);
	free(order);
	free(sprites);
}
