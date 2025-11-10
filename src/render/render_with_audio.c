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
		free(audio);
		return ;
	}
	if (ma_sound_init_from_file(&audio->engine, "bg.wav", MA_SOUND_FLAG_LOOPING,
			NULL, NULL, &audio->bg_music) != MA_SUCCESS)
	{
		ft_putendl_fd("Error cargando bg.wav", 2);
		ma_engine_uninit(&audio->engine);
		free(audio);
		return ;
	}
	ma_sound_start(&audio->bg_music);
	*audio_ptr = audio; // Guardamos el puntero en g->audio
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
