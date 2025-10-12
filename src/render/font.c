/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 17:14:32 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/12 20:00:00 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#define NUM_ROWS 5
#define FONT_ORDER "ABCDEFGHI\nJKLMNOPQR\nSTUVWXYZ\n0123456789\n.:,;+-?/*."

static void	set_bitmap(t_font *f, t_char_bitmap *cb, t_pos rc)
{
	t_pos	pos;

	cb->width = f->char_w;
	cb->height = f->char_h;
	cb->pixels = malloc(sizeof(unsigned int) * f->char_w * f->char_h);
	if (!cb->pixels)
		ft_error_exit("Error: malloc failed for font bitmap\n");
	pos.y = -1;
	while (++pos.y < f->char_h)
	{
		pos.x = -1;
		while (++pos.x < f->char_w)
		{
			cb->pixels[pos.y * f->char_w + pos.x] = *(unsigned int *)(f->addr
					+ (rc.y * f->char_h + pos.y) * f->line_len + (rc.x
						* f->char_w + pos.x) * (f->bpp / 8));
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
	{
		write(2, "Error: font image not found\n", 28);
		exit(1);
	}
	f->addr = mlx_get_data_addr(f->img, &f->bpp, &f->line_len, &f->endian);
	f->char_h = f->char_h / NUM_ROWS;
	f->char_w = f->char_w / 9; // fila más larga = 9 letras
	create_char_bitmap(f);
}

void	put_char(t_game *g, char c, int x, int y)
{
	t_char_bitmap	*cb;
	t_pos			d;
	t_pos			src;
	t_pos			pos;

	cb = &g->font.chars[(int)c];
	if (!cb->pixels)
		return ;
	d.x = (int)(cb->width * g->font.scale);
	d.y = (int)(cb->height * g->font.scale);
	pos.y = -1;
	while (++pos.y < d.y)
	{
		src.y = (int)(pos.y / g->font.scale);
		pos.x = -1;
		while (++pos.x < d.x)
		{
			src.x = (int)(pos.x / g->font.scale);
			if (cb->pixels[src.y * cb->width + src.x] != COLOR_NONE)
				put_pixel(g, x + pos.x, y + pos.y, cb->pixels[src.y * cb->width
					+ src.x]);
		}
	}
}

void	render_text(t_game *g, char *str, t_pos pos)
{
	int	advance;
	int	i;

	advance = (int)(g->font.char_w * g->font.scale);
	i = -1;
	while (str[++i])
		put_char(g, str[i], pos.x + i * advance, pos.y);
}
