#include "mlx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 480
#define MAP_W 10
#define MAP_H 8

char *map[] = {
    "11111111111111111111",
    "1E000000000000000001",
    "11110111011111111001",
    "10000001000000001001",
    "10111111101111111001",
    "10000000000000000001",
    "10111111111111111111",
    "11111111111111111111"
};


typedef struct s_tex {
    void *img;
    char *addr;
    int   width;
    int   height;
    int   bpp;
    int   line_len;
    int   endian;
} t_tex;

typedef struct s_game {
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;

    double posX, posY;
    double dirX, dirY;
    double planeX, planeY;

    t_tex floor;
    t_tex ceiling;
    t_tex wall_north;
    t_tex wall_south;
    t_tex wall_east;
    t_tex wall_west;
} t_game;

static inline int clamp_int(int v, int a, int b) {
    if (v < a) return a;
    if (v > b) return b;
    return v;
}

void put_pixel(t_game *g, int x, int y, int color)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    char *dst = g->addr + (y * g->line_len + x * (g->bpp / 8));
    *(unsigned int*)dst = (unsigned int)color;
}

void load_texture(t_game *g, t_tex *tex, char *path)
{
    tex->img = mlx_xpm_file_to_image(g->mlx, path, &tex->width, &tex->height);
    if (!tex->img)
    {
        fprintf(stderr, "Error: could not load texture %s\n", path);
        exit(1);
    }
    tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len, &tex->endian);
    if (tex->width <= 0 || tex->height <= 0) {
        fprintf(stderr, "Error: texture %s has invalid size\n", path);
        exit(1);
    }
}

void draw_floor_and_ceiling(t_game *g)
{
    /* empezamos en HEIGHT/2 + 1 para evitar p == 0 */
    for (int y = HEIGHT/2 + 1; y < HEIGHT; y++)
    {
        double rayDirX0 = g->dirX - g->planeX;
        double rayDirY0 = g->dirY - g->planeY;
        double rayDirX1 = g->dirX + g->planeX;
        double rayDirY1 = g->dirY + g->planeY;

        int p = y - HEIGHT / 2;
        if (p == 0) continue; /* seguro, aunque no debería pasar */ 
        double posZ = 0.5 * HEIGHT;
        double rowDistance = posZ / (double)p;

        double floorX = g->posX + rowDistance * rayDirX0;
        double floorY = g->posY + rowDistance * rayDirY0;

        double stepX = rowDistance * (rayDirX1 - rayDirX0) / (double)WIDTH;
        double stepY = rowDistance * (rayDirY1 - rayDirY0) / (double)WIDTH;

        for (int x = 0; x < WIDTH; ++x)
        {
            int cellX = (int)floorX;
            int cellY = (int)floorY;

            /* seguridad por si las texturas tienen tamaño 0 (ya comprobado al cargar) */
            int tx = (int)( (floorX - cellX) * g->floor.width );
            int ty = (int)( (floorY - cellY) * g->floor.height );

            /* clamp por si hay rounding negativo/extraño */
            tx = clamp_int(tx, 0, g->floor.width - 1);
            ty = clamp_int(ty, 0, g->floor.height - 1);

            int color = *(unsigned int *)(g->floor.addr + ty * g->floor.line_len + tx * (g->floor.bpp / 8));
            put_pixel(g, x, y, color);

            /* techo espejo */
            int ctx = clamp_int(tx, 0, g->ceiling.width - 1);
            int cty = clamp_int(ty, 0, g->ceiling.height - 1);
            color = *(unsigned int *)(g->ceiling.addr + cty * g->ceiling.line_len + ctx * (g->ceiling.bpp / 8));
            put_pixel(g, x, HEIGHT - y - 1, color);

            floorX += stepX;
            floorY += stepY;
        }
    }
}

int render(t_game *g)
{
    /* limpiar pantalla */
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            put_pixel(g, x, y, 0x000000);

    draw_floor_and_ceiling(g);

    for (int x = 0; x < WIDTH; x++) {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = g->dirX + g->planeX * cameraX;
        double rayDirY = g->dirY + g->planeY * cameraX;

        int mapX = (int)g->posX;
        int mapY = (int)g->posY;

        double sideDistX, sideDistY;
        /* proteger divisiones por cero */
        double deltaDistX = (rayDirX == 0.0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0.0) ? 1e30 : fabs(1.0 / rayDirY);
        double perpWallDist = 0.0;

        int stepX, stepY;
        int hit = 0, side = 0;

        if (rayDirX < 0) { stepX = -1; sideDistX = (g->posX - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0 - g->posX) * deltaDistX; }

        if (rayDirY < 0) { stepY = -1; sideDistY = (g->posY - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0 - g->posY) * deltaDistY; }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            /* PROTECCIÓN: fuera del mapa -> tratamos como hit y salimos */
            if (mapX < 0 || mapX >= MAP_W || mapY < 0 || mapY >= MAP_H) {
                hit = 1;
                break;
            }

            if (map[mapY][mapX] == '1') hit = 1;
        }

        if (side == 0)
            perpWallDist = (rayDirX == 0.0) ? 1e-6 : (mapX - g->posX + (1 - stepX) / 2.0) / rayDirX;
        else
            perpWallDist = (rayDirY == 0.0) ? 1e-6 : (mapY - g->posY + (1 - stepY) / 2.0) / rayDirY;

        if (perpWallDist <= 0.0) perpWallDist = 1e-6; /* protección extra */

        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        double wallX = (side == 0) ? g->posY + perpWallDist * rayDirY
                                   : g->posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        t_tex *tex;
        if (side == 0) {
            if (rayDirX > 0)
                tex = &g->wall_west;
            else
                tex = &g->wall_east;
        } else {
            if (rayDirY > 0)
                tex = &g->wall_north;
            else
                tex = &g->wall_south;
        }

        int texX = (int)(wallX * (double)tex->width);
        if (texX < 0) texX = 0;
        if (texX >= tex->width) texX = tex->width - 1;

        if ((side == 0 && rayDirX > 0) || (side == 1 && rayDirY < 0))
            texX = tex->width - texX - 1;

        for (int y = drawStart; y <= drawEnd; y++) {
            int d = y * 256 - HEIGHT * 128 + lineHeight * 128;
            int texY = ((d * tex->height) / lineHeight) / 256;
            texY = clamp_int(texY, 0, tex->height - 1);
            int color = *(unsigned int *)(tex->addr + texY * tex->line_len + texX * (tex->bpp / 8));
            put_pixel(g, x, y, color);
        }
    }

    mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
    return 0;
}

static void init_player(t_game *g)
{
    for (int y = 0; y < MAP_H; y++)
    {
        for (int x = 0; x < MAP_W; x++)
        {
            char c = map[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'P')
            {
                g->posX = x + 0.5;
                g->posY = y + 0.5;
                if (c == 'N') { g->dirX = 0; g->dirY = -1; g->planeX = 0.66; g->planeY = 0; }
                if (c == 'S') { g->dirX = 0; g->dirY =  1; g->planeX = -0.66; g->planeY = 0; }
                if (c == 'E') { g->dirX = 1; g->dirY =  0; g->planeX = 0; g->planeY = 0.66; }
                if (c == 'W') { g->dirX = -1; g->dirY = 0; g->planeX = 0; g->planeY = -0.66; }
                if (c == 'P') { g->dirX = -1; g->dirY = 0; g->planeX = 0; g->planeY = 0.66; }
                return;
            }
        }
    }
    fprintf(stderr, "Error: no player spawn (N/S/E/W/P) found in map\n");
    exit(1);
}

int main(void)
{
    t_game g = {0};

    g.mlx = mlx_init();
    if (!g.mlx) { fprintf(stderr, "mlx_init failed\n"); return 1; }

    g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Cub3D");
    if (!g.win) { fprintf(stderr, "mlx_new_window failed\n"); return 1; }

    g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
    if (!g.img) { fprintf(stderr, "mlx_new_image failed\n"); return 1; }
    g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_len, &g.endian);

    init_player(&g);

    /* Carga texturas: ajusta paths según tus archivos */
    load_texture(&g, &g.floor, "./textures/floor.xpm");
    load_texture(&g, &g.ceiling, "./textures/ceiling.xpm");
    load_texture(&g, &g.wall_north, "./textures/wall_inner_left.xpm");
    load_texture(&g, &g.wall_south, "./textures/wall_inner_right.xpm");
    load_texture(&g, &g.wall_east, "./textures/wall_inner_right.xpm");
    load_texture(&g, &g.wall_west, "./textures/wall_inner_left.xpm");

    mlx_loop_hook(g.mlx, render, &g);
    mlx_loop(g.mlx);

    return 0;
}
