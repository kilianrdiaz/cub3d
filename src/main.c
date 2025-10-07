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

int main(int argc, char **argv)
{
    t_game *game;
    char **content;
    int fd;

    game = ft_calloc(1, sizeof(t_game));

    if (!game)
        return (1);

    game->elems = NULL;
    
    parse_arguments(argc, argv);

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        error_handler(2);

    content = NULL;
    content = read_file(fd);

    int i = 0;

    while (content[i])
    {
        printf("%s\n",content[i]);
        i++;
    }


    parse_file(game, content);

    i = 0;
    while (game->elems && game->elems[i])
    {
        printf("ID: %s\n", game->elems[i]->id);
        if (game->elems[i]->texture)
            printf("Texture loaded\n");
        else
            printf("Color: %u\n", game->elems[i]->color);
        i++;
    }

    printf("\n--- MAPA GUARDADO ---\n");
    i = 0;
    while (game->map.map && game->map.map[i])
    {
        printf("%s\n", game->map.map[i]);
        i++;
    }
    printf("Altura: %d, Ancho máximo: %d\n", game->map.height, game->map.width);
    //load map textures, colors...
    return (0);
}