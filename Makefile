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
INCLUDES := -I./ -I./ft_irc/include

# List all your source files in the SRC variable
SRC :=	ft_irc/main.cpp		\
		ft_irc/Src/Server.cpp	\
		ft_irc/Src/Client.cpp	\
		ft_irc/Src/Channel.cpp \
		ft_irc/Commands/User.cpp \
		ft_irc/Commands/Pass.cpp \
		ft_irc/Commands/Nick.cpp \
		ft_irc/Commands/Quit.cpp \
		ft_irc/Commands/Join.cpp \
		ft_irc/Commands/List.cpp \
		ft_irc/Commands/Whois.cpp \
		ft_irc/Commands/Who.cpp \
		# ft_irc/Commands/Kick.cpp \
		# ft_irc/Commands/Mode.cpp \
		# ft_irc/Commands/Msg.cpp \
		# ft_irc/Commands/Privmsg.cpp \
		

# Generate the list of object files based on source files
OBJ := $(SRC:.cpp=.o)

NAME := ircserv

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CXXFLAGS) $(INCLUDES) -o $(NAME) $(OBJ)

# Compile each source file into an object file
%.o: %.cpp
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
