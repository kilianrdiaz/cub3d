# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 17:31:45 by alejhern          #+#    #+#              #
#    Updated: 2025/12/05 17:31:52 by alejhern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================
# ⚙️ COMPILACIÓN
# ================================
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g -Iinc
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer
NAME    = cub3D
NAME_AUDIO = cub3DA
VICTUS  = -DFRAMES_PER_SECOND=30

DIR_OBJ_DEPS = obj_deps
OBJ_DIR      = $(DIR_OBJ_DEPS)/$(NAME)
OBJ_DIR_A    = $(DIR_OBJ_DEPS)/$(NAME_AUDIO)

# ================================
# 🎨 OBJETOS RENDER
# ================================
RENDER_OBJ       = $(OBJ_DIR)/render/render.o
RENDER_AUDIO_OBJ = $(OBJ_DIR_A)/render/render_with_audio.o

SRC_RENDER       = src/render/render.c
SRC_RENDER_AUDIO = src/render/render_with_audio.c

# ================================
# 📂 FUENTES
# ================================
SRCS = src/utils/parsing.c \
       src/utils/render.c \
	   src/utils/moves.c \
       src/utils/ray_map.c \
       src/utils/score.c \
       src/parsing/map_textures.c \
       src/parsing/map_checks.c \
       src/parsing/map.c \
       src/parsing/sprites.c \
       src/parsing/file.c \
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

OBJ   = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJ_A = $(patsubst src/%.c,$(OBJ_DIR_A)/%.o,$(SRCS))

# ================================
# 🧱 LIBFT
# ================================
LIBFT_DIR = libs/libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a
LIBFT_REPO = https://github.com/alejhern/libft.git
LIBFT = -L$(LIBFT_DIR) -lft

# ================================
# 🖼️ MINILIBX
# ================================
MLX_DIR = libs/minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# ================================
# 🎵 MINIAUDIO
# ================================
MINIAUDIO_DIR  = libs/miniaudio
MINIAUDIO_FILE = $(MINIAUDIO_DIR)/miniaudio.h
MINIAUDIO_INC  = -I$(MINIAUDIO_DIR)

# **************************************************************************** #
#                                 REGLAS                                       #
# **************************************************************************** #

# -----------------------------
# Compilación normal
# -----------------------------
all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

$(NAME): $(OBJ) $(RENDER_OBJ)
	@echo "🎨 Linkando render normal..."
	$(CC) $(OBJ) $(RENDER_OBJ) -o $(NAME) $(MLX) $(LIBFT)
	@echo "✅ Compilación normal finalizada."

$(RENDER_OBJ): $(SRC_RENDER) inc/cub3d.h Makefile
	@mkdir -p $(dir $@)
	@echo "🎨 Compilando render normal..."
	$(CC) $(CFLAGS) -MMD -MP -c $(SRC_RENDER) -o $@

# -----------------------------
# Compilación render con audio
# -----------------------------
$(RENDER_AUDIO_OBJ): $(SRC_RENDER_AUDIO) inc/cub3d.h inc/audio.h Makefile
	@mkdir -p $(dir $@)
	@echo "🔊 Compilando render con audio..."
	$(CC) $(CFLAGS) $(MINIAUDIO_INC) -MMD -MP -c $(SRC_RENDER_AUDIO) -o $@

$(NAME_AUDIO): $(OBJ_A) $(RENDER_AUDIO_OBJ)
	@echo "🔊 Linkando render con audio..."
	$(CC) $(OBJ_A) $(RENDER_AUDIO_OBJ) -o $(NAME_AUDIO) $(MLX) $(LIBFT)
	@echo "✅ Compilación con audio finalizada."

miniaudio: $(MINIAUDIO_FILE) $(LIBFT_LIB) $(MLX_LIB) $(NAME_AUDIO)

# -----------------------------
# Victus (modo optimizado)
# -----------------------------
victus: CFLAGS += $(VICTUS)
victus: miniaudio

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
		git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIR); \
	fi
	@echo "⚙️  Compilando MiniLibX..."
	@$(MAKE) -C $(MLX_DIR) > /dev/null

$(MINIAUDIO_FILE):
	@if [ ! -d "$(MINIAUDIO_DIR)" ];then \
		echo "📥 Clonando Miniaudio..."; \
		git clone https://github.com/mackron/miniaudio.git $(MINIAUDIO_DIR); \
	fi

# ===========================
#   🧹 LIMPIEZA
# ===========================
clean:
	@rm -rf $(DIR_OBJ_DEPS)
	@make -C $(LIBFT_DIR) clean || true
	@echo "🧹 Archivos objeto eliminados."

fclean: clean
	@rm -f $(NAME) $(NAME_AUDIO)
	@rm -rf libs
	@make -C $(LIBFT_DIR) fclean || true
	@echo "🧼 Limpieza completa: binarios y librerías externas eliminados."

re: fclean all
re_miniaudio: fclean miniaudio

# -----------------------------
# 🧪 Compilación con Sanitizer
# -----------------------------
san: export CFLAGS := $(CFLAGS) $(ASAN_FLAGS)
san: export MLX := $(MLX) $(ASAN_FLAGS)
san: export LIBFT := $(LIBFT) $(ASAN_FLAGS)
san:
	@echo "🧪 Activando AddressSanitizer..."
	@$(MAKE) fclean
	@$(MAKE) all
	@echo "🧪 Compilación con ASan completada."

# -----------------------------
# Compilación de cada objeto
# -----------------------------
$(OBJ_DIR)/%.o: src/%.c inc/cub3d.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Objetos con audio
$(OBJ_DIR_A)/%.o: src/%.c inc/cub3d.h inc/audio.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MINIAUDIO_INC) -MMD -MP -c $< -o $@

# ===========================
#   📄 INCLUDE DEPENDENCIAS
# ===========================
-include $(OBJ:.o=.d)
-include $(OBJ_A:.o=.d)
-include $(RENDER_OBJ:.o=.d)
-include $(RENDER_AUDIO_OBJ:.o=.d)

.PHONY: all miniaudio clean fclean re re_miniaudio san victus
