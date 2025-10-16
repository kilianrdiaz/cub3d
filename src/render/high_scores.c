/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   high_scores.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:02:07 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 16:02:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/cub3d.h"

static void  add_buttons(t_game *g, t_sprite *alphabet, int index, t_pos pos)
{
    pos.y += g->font.char_h * g->font.scale - 10;
    g->font.scale = 0.75;
    alphabet[index].x = pos.x;
    alphabet[index].y = pos.y;
    render_text(g, "DEL", pos);
    index++;
    pos.x += g->font.char_w * 3 * g->font.scale + 10;
    alphabet[index].x = pos.x;
    alphabet[index].y = pos.y;
    render_text(g, "ENTER", pos);
}

static t_sprite *print_alphabet(t_game *game, t_tex score_panel)
{
    char x;
    int index;
    t_sprite *alphabet;
    t_pos pos;

    alphabet = malloc(sizeof(t_sprite) * 28);
    if (!alphabet)
        return (NULL);
    pos.x = (WIDTH - score_panel.width) / 2 - 60;
    pos.y = (HEIGHT - score_panel.height) / 2 + 120;
    index = 0;
    x = 'A';
    while (x <= 'Z')
    {
        alphabet[index].x = pos.x;
        alphabet[index].y = pos.y;
        render_text(game, &x, pos);
        if ((x - 'A' + 1) % 9 == 0)
        {
            pos.x = (WIDTH - score_panel.width) / 2 - 60;
            pos.y += game->font.char_h * game->font.scale + 10;
        }
        else
            pos.x += game->font.char_w * game->font.scale + 10;
        index++;
        x++;
    }
    add_buttons(game, alphabet, index, pos);
    return (alphabet);
}

int    register_score(t_game *g)
{
    t_tex    score_panel;
    t_sprite *alphabet;
    t_pos    pos;

    g->render_state = HIGH_SCORE;
    load_texture(g, &score_panel, "./textures/score_panel.xpm");
    draw_fullscreen_image(g, &score_panel);
    pos.x = (WIDTH - score_panel.width) / 2;
    pos.y = (HEIGHT - score_panel.height) / 2;
    g->font.scale = 1.5;
    render_text(g, "ENTER NAME", pos);
    g->font.scale = 3.5;
    alphabet = print_alphabet(g, score_panel);
    free(alphabet);
    g->font.scale = 2.5;
    render_text(g, "-----", (t_pos){pos.x, pos.y + 700});
    draw_hand(g);
    mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
    mlx_destroy_image(g->mlx, score_panel.img);
    return (0);
}