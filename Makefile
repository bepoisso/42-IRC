# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bepoisso <bepoisso@student.42perpignan.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/14 15:08:07 by bepoisso          #+#    #+#              #
#    Updated: 2025/07/14 15:38:27 by bepoisso         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:
#_________________COLORS_________________

PURPLE 	= \033[0;35m
YELLOW 	= \033[0;93m
RED		= \033[0;31m
RESET 	= \033[0m

#_________________VARIABLE_________________

NAME = ircserv

CC			= c++
CFLAGS		= -Wall -Wextra -Werror -g -std=c++98 -I$(INC_DIR)

#_________________DIRECTORY_________________
INC_DIR	= includes/
SRC_DIR	= srcs/
OBJ_DIR	= objs/

#_________________FILES_________________

SRC_FILES	=	bot.cpp						\
				channel.cpp					\
				channelGlobalCommand.cpp	\
				channelOpCommand.cpp		\
				logs.cpp					\
				main.cpp					\
				network_tools.cpp			\
				packetChecker.cpp			\
				parser.cpp					\
				parserBot.cpp				\
				parserUtils.cpp				\
				stop_process.cpp			\
				user.cpp

				


SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))

#_________________RULES_________________

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Create Program		✅"
	@echo "$(RED)   _____ ________           .___  __________  _________  $(RESET)"
	@echo "$(RED)  /  |  |\_____  \          |   | \______   \ \_   ___ \ $(RESET)"
	@echo "$(RED) /   |  |_/  ____/   ______ |   |  |       _/ /    \  \/ $(RESET)"
	@echo "$(RED)/    ^   /       \  /_____/ |   |  |    |   \ \     \____$(RESET)"
	@echo "$(RED)\____   |\_______ \         |___|  |____|_  /  \______  /$(RESET)"
	@echo "$(RED)     |__|        \/                       \/          \/ $(RESET)"
	@echo "$(PURPLE)                               by hhecquet 🏢, heperez 🔫, bepoisso 🐟  $(RESET)"
	@echo "$(PURPLE)🤖 To execute : ./ircserv <port> <password> 🤖$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clean Program Objs	✅"

fclean: clean
	@rm -f $(NAME)
	@echo "Clean Program		✅"

re: fclean all

.PHONY: all clean fclean re