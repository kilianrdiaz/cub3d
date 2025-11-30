# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/11/30 13:21:58 by kroyo-di         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#                                   CUB3D                                      #
# **************************************************************************** #

# ================================
# ⚙️ COMPILACIÓN
# ================================
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g -Iinc
NAME    = cub3d
VICTUS  = -DFRAMES_PER_SECOND=30

# ================================
# 📂 DIRECTORIOS Y FUENTES
# ================================
OBJ_DIR = obj

SRCS    = src/utils/parsing.c  \
		  src/utils/render.c    \
		  src/utils/ray_map.c \
		  src/utils/score.c   \
		  src/parsing/map_textures.c   \
		  src/parsing/map_checks.c   \
		  src/parsing/map.c   \
		  src/parsing/sprites.c   \
		  src/parsing/file.c   \
		  src/render/player.c \
		  src/render/map.c \
		  src/render/sprite.c \
		  src/render/font.c \
		  src/render/intro.c \
		  src/render/load_level.c \
		  src/render/game.c \
		  src/render/timer.c \
		  src/render/stats.c \
		  src/render/display_scores.c \
		  src/render/high_scores.c \
		  src/render/display_health.c \
		  src/render/minimap/minimap.c \
		  src/render/minimap/draw_minimap.c \
		  src/render/minimap/minimap_sprites.c \
		  src/render/minimap/player.c \
		  src/render/minimap/reveal_minimap.c \
		  src/moves/spider.c \
		  src/moves/spider_attack.c \
		  src/moves/lizards.c \
		  src/moves/key_hooks.c \
		  src/moves/mark_score.c \
		  src/moves/damage_system.c \
		  src/close_program.c \
		  src/main.c

# ================================
# 🧱 LIBFT
# ================================
LIBFT_DIR = libs/libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a
LIBFT_REPO = https://github.com/alejhern/libft.git
LIBFT   = -L$(LIBFT_DIR) -lft

# ================================
# 🖼️ MINILIBX
# ================================
MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_DIR = libs/minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# ================================
# 🎵 MINIAUDIO (header-only)
# ================================
MINIAUDIO_DIR  = libs/miniaudio
MINIAUDIO_REPO = https://github.com/mackron/miniaudio.git
MINIAUDIO_LIB  = $(MINIAUDIO_DIR)/miniaudio.h
MINIAUDIO_INC  = -I$(MINIAUDIO_DIR)

# ================================
#   OBJETOS DINÁMICOS
# ================================
OBJ_RENDER     = $(OBJ_DIR)/render/render.o
OBJ_RENDER_SRC ?= src/render/render.c
OBJ             = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS)) $(OBJ_RENDER)

# **************************************************************************** #
#                                 REGLAS                                       #
# **************************************************************************** #

# -----------------------------
# Compilación normal
# -----------------------------
all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

# -----------------------------
# Compilación con audio
# -----------------------------
miniaudio: $(MINIAUDIO_LIB)
	@echo "🔊 Compilando con audio..."
	$(MAKE) OBJ_RENDER_SRC=src/render/render_with_audio.c CFLAGS+="$(CFLAGS) $(MINIAUDIO_INC)" all

# -----------------------------
# Victus (modo optimizado)
# -----------------------------
victus: CFLAGS += $(VICTUS)
victus: miniaudio

# -----------------------------
# Compilación final
# -----------------------------
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(MLX) $(LIBFT)

# -----------------------------
# Compilación de cada objeto
# -----------------------------
$(OBJ_DIR)/%.o: src/%.c ./inc/cub3d.h ./inc/audio.h Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# render.o (usa el SRC dinámico correcto)
$(OBJ_DIR)/render/render.o: $(OBJ_RENDER_SRC) ./inc/cub3d.h ./inc/audio.h Makefile
	@mkdir -p $(dir $@)
	@echo "🎨 Compilando render desde: $(OBJ_RENDER_SRC)"
	$(CC) $(CFLAGS) -c $(OBJ_RENDER_SRC) -o $@

# ===========================
#   🧰 LIBRERÍAS EXTERNAS
# ===========================
$(LIBFT_LIB):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		echo "📥 Clonando Libft..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@make -C $(LIBFT_DIR)
	
$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "📥 Clonando MiniLibX..."; \
		git clone $(MLX_REPO) $(MLX_DIR); \
	fi
	@echo "⚙️  Compilando MiniLibX..."
	@$(MAKE) -C $(MLX_DIR) > /dev/null


$(MINIAUDIO_LIB):
	@if [ ! -f "$(MINIAUDIO_LIB)" ]; then \
		echo "📥 Clonando Miniaudio..."; \
		git clone $(MINIAUDIO_REPO) $(MINIAUDIO_DIR); \
	fi

# ===========================
#   🧹 LIMPIEZA
# ===========================
clean:
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) clean; \
	fi
	rm -rf $(OBJ_DIR)
	@echo "🧹 Archivos objeto eliminados."

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then \
		$(MAKE) -C $(LIBFT_DIR) fclean; \
	fi
	rm -rf libs
	rm -f $(NAME)
	@echo "🧼 Limpieza completa."

re: fclean all

.PHONY: all clean fclean re victus miniaudio
