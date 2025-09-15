# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/09/15 14:52:40 by alejhern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

NAME    = cub3d
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
MLX     = -lmlx -lXext -lX11 -lm
SRC     = main.c
OBJ     = $(SRC:.c=.o)

LIB_DIR = libft/
LIBFT = $(LIB_DIR)libft.a

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) libft Makefile
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(MLX)

$(LIBFT):
	@if [ ! -d "$(LIB_DIR)" ]; then \
		git clone https://github.com/alejhern/libft.git $(LIB_DIR); \
	fi
	@make -C $(LIB_DIR)

clean:
	@make -C $(LIB_DIR) clean
	rm -f $(OBJ)

fclean: clean
	@make -C $(LIB_DIR) fclean
	rm -f $(NAME)

re: fclean all
