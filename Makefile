# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/09/17 21:22:58 by kroyo-di         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

NAME    = cub3d
CC      = cc
CFLAGS = -Wall -Werror -Wextra -I inc -I $(LIB_DIR)
MLX     = -lmlx -lXext -lX11 -lm
SRCS    = src/parsing/error.c	\
		  src/parsing/parser.c   \
		  src/parsing/utils.c   \
		  src/parsing/parse_elems.c   \
		  src/parsing/map.c   \
		  src/parsing/map_checking.c   \
		  src/main.c

OBJ     = $(SRCS:.c=.o)

LIB_DIR = libft/
LIBFT = $(LIB_DIR)libft.a

NAME    = cub3d
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I./inc -I$(LIB_DIR) -g
MLX     = -lmlx -lXext -lX11 -lm

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT) Makefile
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT)

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