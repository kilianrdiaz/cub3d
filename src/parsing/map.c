#include "../../inc/cub3d.h"

/*void	get_map(t_game *game, char **content, int start_index)
{
	int	i;
	int map_start;
	int map_end;

	i = start_index;
	map_start = start_index;

	// 1️⃣ Encuentra el final del mapa
	while (content[i] && is_map_str(content[i]))
		i++;
	map_end = i;

	// 2️⃣ Comprueba que después del mapa no haya líneas no vacías
	while (content[i])
	{
		if (!is_empty_line(content[i]))
		{
			printf("Error: línea no vacía después del mapa: '%s'\n", content[i]);
			error_handler(3);
		}
		i++;
	}

	// 3️⃣ Copia el mapa al struct del juego
	game->map.height = map_end - map_start;
	game->map.map = malloc((game->map.height + 1) * sizeof(char *));
	if (!game->map.map)
		error_handler(3);

	i = 0;
	while (i < game->map.height)
	{
		game->map.map[i] = ft_strdup(content[map_start + i]);
		i++;
	}
	game->map.map[i] = NULL;

	// 4️⃣ Calcular ancho máximo (útil para render o validación)
	game->map.width = 0;
	i = 0;
	while (game->map.map[i])
	{
		int len = ft_strlen(game->map.map[i]);
		if (len > game->map.width)
			game->map.width = len;
		i++;
	}

	printf("Mapa guardado: %d filas, %d columnas máx.\n", game->map.height, game->map.width);
}*/
