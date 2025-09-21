/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:50 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 13:56:52 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	clamp_int(int v, int a, int b)
{
	if (v < a)
		return (a);
	if (v > b)
		return (b);
	return (v);
}

void	clean_screen(t_game *g)
{
	t_pos	it;

	it.y = -1;
	while (++it.y < HEIGHT)
	{
		it.x = -1;
		while (++it.x < WIDTH)
			put_pixel(g, it.x, it.y, 0x000000);
	}
}

void	put_pixel(t_game *g, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = g->addr + (y * g->line_len + x * (g->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

void	load_texture(t_game *g, t_tex *tex, char *path)
{
	tex->img = mlx_xpm_file_to_image(g->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		ft_printf_fd(STDERR_FILENO, "Error: could not load texture %s\n", path);
		exit(1);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	if (tex->width <= 0 || tex->height <= 0)
	{
		ft_printf_fd(STDERR_FILENO, "Error: texture %s has invalid size\n",
			path);
		exit(1);
	}
}

t_tex	*get_texture_wall(t_game *g, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->dir_x0 > 0)
			return (&g->wall_west);
		return (&g->wall_east);
	}
	else
	{
		if (ray->dir_y0 > 0)
			return (&g->wall_south);
		return (&g->wall_north);
	}
}
