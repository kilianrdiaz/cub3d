/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_with_audio.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/10 15:41:27 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/audio.h"
#include "../../inc/cub3d.h"

static void	clean_audio(t_audio *audio)
{
	if (!audio)
		return ;
	ma_sound_stop(&audio->bg_music);
	ma_sound_uninit(&audio->bg_music);
	ma_sound_uninit(&audio->lizard);
	ma_sound_uninit(&audio->spiderweb);
	ma_sound_uninit(&audio->game_over);
	ma_engine_uninit(&audio->engine);
	free(audio);
}

static void	audio_init(void **audio_ptr)
{
	t_audio	*audio;

	if (*audio_ptr) // Ya inicializado
		return ;
	audio = ft_calloc(1, sizeof(t_audio));
	if (!audio)
		return ;
	if (ma_engine_init(NULL, &audio->engine) != MA_SUCCESS)
	{
		ft_putendl_fd("Error inicializando el motor de audio", 2);
		return (clean_audio(audio));
	}
	if (ma_sound_init_from_file(&audio->engine, "audios/bg.wav",
			MA_SOUND_FLAG_LOOPING, NULL, NULL, &audio->bg_music) != MA_SUCCESS)
		ft_putendl_fd("Error cargando bg.wav", 2);
	if (ma_sound_init_from_file(&audio->engine, "audios/lizard.wav", 0, NULL,
			NULL, &audio->lizard) != MA_SUCCESS)
		ft_putendl_fd("Error cargando lizard.wav", 2);
	if (ma_sound_init_from_file(&audio->engine, "audios/web.wav", 0, NULL, NULL,
			&audio->spiderweb) != MA_SUCCESS)
		ft_putendl_fd("Error cargando spiderweb.wav", 2);
	if (ma_sound_init_from_file(&audio->engine, "audios/gameover.wav", 0, NULL,
			NULL, &audio->game_over) != MA_SUCCESS)
		ft_putendl_fd("Error cargando gameover.wav", 2);
	ma_sound_start(&audio->bg_music);
	*audio_ptr = audio; // Guardamos el puntero en g->audio
}

static void	manage_sound_effects(t_game *g)
{
	t_audio		*audio;
	t_sprite	**sprite_list;
	int			i;

	audio = (t_audio *)g->audio;
	if (!audio)
		return ;
	if (g->render_state == GAME_OVER && g->timer < 10)
		return (ma_sound_start(&audio->game_over), (void)0);
	else if (g->render_state == GAME_OVER)
		return ;
	if (g->spider.state == ATTACKING)
		ma_sound_start(&audio->spiderweb);
	sprite_list = get_sprites(*g);
	i = -1;
	while (sprite_list && sprite_list[++i])
	{
		if (sprite_list[i]->type == LIZARD
			&& sprite_list[i]->state == ATTACKING)
			ma_sound_start(&audio->lizard);
	}
	free(sprite_list);
}

static void	game(t_game *g)
{
	update_player_position(g);
	clean_screen(g);
	render_floor_and_ceiling(g);
	render_wall(g);
	render_sprites(g);
	move_lizards(g);
	draw_hand(g, GAME_WIDTH / 2);
	draw_minimap(g);
	render_stats(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	update_timer(g);
	update_bombs(g);
	if (g->spider.state == ATTACKING)
		spider_attack(g);
}

int	render(t_game *g)
{
	t_audio	*audio;

	audio_init(&g->audio);
	manage_sound_effects(g);
	audio = (t_audio *)g->audio;
	if (g->render_state == INTRO)
		show_intro(g);
	else if (g->render_state == LOAD_LEVEL || g->render_state == NEW_LEVEL
		|| g->render_state == GAME_OVER)
		load_level(g);
	else if (g->render_state == PLAYING)
		game(g);
	else if (g->render_state == HIGH_SCORE
		|| g->render_state == WAITING_FOR_NAME
		|| g->render_state == SCORE_SAVED)
		show_high_scores(g);
	else if (g->render_state == END)
	{
		clean_audio(audio);
		close_program(g);
	}
	return (0);
}
