/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:47:00 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/10 16:47:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static char	**read_file(int fd)
{
	char	**content;
	char	*line;

	content = NULL;
	line = get_next_line(fd);
	if (!line)
		ft_error_exit("Error: Empty file or could not read file\n");
	while (line)
	{
		ft_clean_line(&line);
		ft_append_array((void ***)&content, line);
		line = get_next_line(fd);
	}
	return (content);
}

void	get_info_file(t_game *g, int argc, char **argv)
{
	int		fd;
	char	**content;
	int		len;

	if (argc != 2)
		ft_error_exit("Error: Invalid number of arguments\n");
	len = ft_strlen(argv[1]);
	if (len < 4 || ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0)
		ft_error_exit("Error: Invalid file extension, expected .cub\n");
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		ft_error_exit("Error: Could not open file\n");
	content = read_file(fd);
	if (!content)
		ft_error_exit("Error: Could not read file content\n");
	load_map_textures(g, content);
	g->map = get_map(content);
	if (!g->map)
		ft_error_exit("Error: Could not load map\n");
	ft_free_array((void ***)&content);
	close(fd);
	create_spiderman(g);
	create_sprites(g);
}
