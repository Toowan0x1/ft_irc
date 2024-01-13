# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oel-houm <oel-houm@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/12 12:55:01 by oel-houm          #+#    #+#              #
#    Updated: 2024/01/12 13:04:06 by oel-houm         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC := g++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98
INCLUDES := -I./

SRC := main.cpp
OBJ := $(SRC:.cpp=.o)

NAME := ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJ)

%.o: %.cpp
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

