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
	void		*img;
	char		*addr;
	int			width;
	int			height;
	int			bpp;
	int			line_len;
	int			endian;
}				t_tex;

typedef struct s_map
{
	char	**map;
	int		max_width;
	int		height;
}				t_map;

typedef struct s_elem
{
	t_tex	*texture;
	char	*id;
}				t_elem;

typedef struct s_game
{
	t_map	map;
	t_elem	*elems;
}				t_game;

void    error_handler(int code);
void    parse_arguments(t_game *game, int argc, char **argv);
void	parse_element(t_game *game, char *line);
void	load_texture(t_game *g, t_tex *tex, char *path);
void	parse_file(t_game *game, char **content);

int     ft_isspace(int c);
char	**read_file(int fd);

#endif