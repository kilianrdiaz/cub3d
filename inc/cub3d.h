/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:27:17 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:24:36 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"

typedef struct s_tex
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}			t_tex;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;

	double posX, posY;
	double dirX, dirY;
	double planeX, planeY;

	t_tex	floor;
	t_tex	ceiling;
	t_tex	wall_north;
	t_tex	wall_south;
	t_tex	wall_east;
	t_tex	wall_west;
	t_tex	hand;
}			t_game;

void    error_handler(int code);
void    parse_arguments(int argc, char **argv);
void	parse_element(char *line, char *id);

int     ft_isspace(int c);

#endif