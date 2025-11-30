/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:47:00 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/29 20:39:26 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define O_FAILED "Error: Could not open file %s\n"
#define R_FAILED "Error: Could not read %s content\n"
#define NO_BOMBS "Warning: No bombs found in the map %s\n"
#define MAP_INVALID "Error: Invalid map in %s\n"

static char	**read_file(int fd)
{
	char	**content;
	char	*line;

	content = NULL;
	line = get_next_line(fd);
	if (!line)
		return (NULL);
	while (line)
	{
		ft_clean_line(&line);
		ft_append_array((void ***)&content, line);
		line = get_next_line(fd);
	}
	return (content);
}

int	check_files_extension(int argc, char **argv)
{
	int	i;
	int	len;

	if (argc < 2)
		ft_error_exit("Error: No input files provided\n");
	i = 0;
	while (argv[++i])
	{
		len = ft_strlen(argv[i]);
		if (len < 4 || ft_strncmp(argv[i] + len - 4, ".cub", 4) != 0)
		{
			ft_printf_fd(2, "Error: Invalid file extension for %s, ", argv[i]);
			ft_putendl_fd("Expected .cub", 2);
			return (1);
		}
	}
	return (0);
}

void	get_info_file(t_game *g)
{
	int		fd;
	char	**content;

	if (!ft_memlen(g->levels))
		return (g->render_state = WIN, (void)0);
	free_level(g);
	fd = open(*g->levels, O_RDONLY);
	if (fd == -1)
		return (set_error_parsing(g, O_FAILED, *g->levels));
	content = read_file(fd);
	if (!content)
		return (set_error_parsing(g, R_FAILED, *g->levels));
	load_map_textures(g, content);
	g->map = get_map(content);
	if (!g->map)
		return (set_error_parsing(g, MAP_INVALID, *g->levels));
	ft_free_array((void ***)&content);
	close(fd);
	create_spiderman(g);
	create_sprites(g);
	if (!g->bomb_count)
		set_error_parsing(g, NO_BOMBS, *g->levels);
	if (g->level > 1 && g->render_state == LOAD_LEVEL)
		g->render_state = NEW_LEVEL;
}
