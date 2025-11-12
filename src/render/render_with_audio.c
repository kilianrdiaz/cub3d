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

static void	load_audio(ma_engine *engine, ma_sound *sound,
		const char *file_path, ma_uint32 flags)
{
	if (ma_sound_init_from_file(engine, file_path, flags, NULL, NULL,
			sound) != MA_SUCCESS)
		ft_printf_fd(STDERR_FILENO, "Error loading audio file: %s\n",
			file_path);
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
	load_audio(&audio->engine, &audio->bg_music, "audios/bg.wav",
		MA_SOUND_FLAG_LOOPING);
	ma_sound_start(&audio->bg_music);
	load_audio(&audio->engine, &audio->lizard, "audios/lizard.wav", 0);
	load_audio(&audio->engine, &audio->spiderweb, "audios/web.wav", 0);
	load_audio(&audio->engine, &audio->game_over, "audios/gameover.wav", 0);
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
	else if (g->render_state >= GAME_OVER )
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
		g->game_func(g);
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
