/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:14:32 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/08 16:30:57 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#define NUM_ROWS 5
#define FONT_ORDER "ABCDEFGHI\nJKLMNOPQR\nSTUVWXYZ0\n123456789\n'+-:,.!?/"

static void	set_bitmap(t_font *f, t_char_bitmap *cb, t_pos rc)
{
	t_pos	pos;
	unsigned long i;

	cb->width = f->char_w;
	cb->height = f->char_h;
	cb->pixels = malloc(sizeof(unsigned int) * f->char_w * f->char_h);
	if (!cb->pixels)
		ft_error_exit("Error: malloc failed for font bitmap\n");

	pos.y = -1;
	i = 0;
	while (++pos.y < f->char_h)
	{
		pos.x = -1;
		while (++pos.x < f->char_w)
		{
			size_t src_y = rc.y * f->char_h + pos.y;
			size_t src_x = rc.x * f->char_w + pos.x;
			size_t byte_offset = src_y * f->line_len + src_x * (f->bpp / 8);
			cb->pixels[i++] = *(unsigned int *)(f->addr + byte_offset);
		}
	}
}

static void	create_char_bitmap(t_font *font)
{
	char			**fnt_order;
	int				cols;
	t_char_bitmap	*cb;
	t_pos			pos;

	fnt_order = ft_split(FONT_ORDER, '\n');
	if (!fnt_order)
		ft_error_exit("Error: malloc failed for font bitmap\n");

	pos.y = -1;
	while (++pos.y < NUM_ROWS)
	{
		cols = ft_strlen(fnt_order[pos.y]);
		pos.x = -1;
		while (++pos.x < cols)
		{
			cb = &font->chars[(int)fnt_order[pos.y][pos.x]];
			set_bitmap(font, cb, pos);
		}
	}
	ft_free_array((void ***)&fnt_order);
}

void	load_font(t_game *g, t_font *f, char *path)
{
	f->img = mlx_xpm_file_to_image(g->mlx, path, &f->char_w, &f->char_h);
	if (!f->img)
		ft_error_exit("Error: font image not found\n");

	f->addr = mlx_get_data_addr(f->img, &f->bpp, &f->line_len, &f->endian);
	f->scale = 1.0;
	f->char_h = f->char_h / NUM_ROWS;
	f->char_w = f->char_w / 9;

	create_char_bitmap(f);
}

void	put_char(t_game *g, char c, int x, int y)
{
	t_char_bitmap	*cb;
	t_pos			d;
	t_pos			src;
	t_pos			pos;
	unsigned char	uc = (unsigned char)c;

	if (!g || !g->font.img || g->font.scale <= 0.0)
		return ;
	if (uc >= 128 || !g->font.chars[uc].pixels)
		return ;

	cb = &g->font.chars[uc];
	if (!cb || !cb->pixels || cb->width <= 0 || cb->height <= 0)
		return ;

	d.x = (int)(cb->width * g->font.scale);
	d.y = (int)(cb->height * g->font.scale);
	if (d.x <= 0 || d.y <= 0)
		return ;

	pos.y = -1;
	while (++pos.y < d.y)
	{
		src.y = (int)(pos.y / g->font.scale);
		if (src.y < 0 || src.y >= cb->height)
			continue ;
		pos.x = -1;
		while (++pos.x < d.x)
		{
			src.x = (int)(pos.x / g->font.scale);
			if (src.x < 0 || src.x >= cb->width)
				continue ;
			if (cb->pixels[src.y * cb->width + src.x] != COLOR_NONE)
				put_pixel(g, x + pos.x, y + pos.y, cb->pixels[src.y * cb->width + src.x]);
		}
	}
}

void	render_text(t_game *g, char *str, t_pos pos)
{
	int	advance;
	int	i;

	if (!str)
		return ;
	advance = (int)(g->font.char_w * g->font.scale);
	i = -1;
	while (str[++i])
		put_char(g, str[i], pos.x + i * advance, pos.y);
}
