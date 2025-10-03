/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:03:30 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:10:21 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../inc/cub3d.h"

void get_map(t_game *g, char **content)
{
    //check map 
    while (*content)
    {
        if (ft_strlen(*content) == 0)
            break ;
        ft_append_array((void ***)&g->map.map, ft_strdup(*content));
        content++;
    }
}

int main(int argc, char **argv)
{
    t_game *game;
    char **content;
    int fd;

    game = malloc(sizeof(t_game));
    if (!game)
        return (1);
    
    parse_arguments(game, argc, argv);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    error_handler(2);

    content = NULL;
    content = read_file(fd); 
    parse_file(game, content);

    //load map textures, colors...
    return (0);
}